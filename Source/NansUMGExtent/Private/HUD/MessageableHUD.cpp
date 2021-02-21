// Copyright 2020-present Nans Pellicari (nans.pellicari@gmail.com).
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "HUD/MessageableHUD.h"
#include "Widget/MessageableWidget.h"

AMessageableHUD::AMessageableHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void AMessageableHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!ensureMsgf(MessageWidgetClass != nullptr, TEXT("Should have a widget class to display message"))) return;
	if (!ensureMsgf(GetWorld() != nullptr, TEXT("Should have a world for... everything!!"))) return;

	MessageWidget = CreateWidget<UMessageableWidget>(GetWorld(), MessageWidgetClass);
	MessageWidget->AddToViewport();
	MessageWidget->SetVisibility(ESlateVisibility::Collapsed);
	MessageWidget->OnNativeEndDisplayMessage().AddUObject(this, &AMessageableHUD::WhenEndDisplayMessage);
}

void AMessageableHUD::WhenEndDisplayMessage()
{
	if (bDebug == true && !CurrentMessage.Message.IsEmpty())
	{
		UE_LOG(
			LogTemp,
			Log,
			TEXT("Message \"%s\" should not be displayed anymore"),
			*CurrentMessage.Message.ToString()
		);
	}

	CurrentMessage.Clear();
	if (!MessagesQueues.IsEmpty())
	{
		CurrentMessage = MessagesQueues.GetMessage();

		if (MessageWidget != nullptr)
		{
			if (bDebug == true) UE_LOG(
				LogTemp,
				Log,
				TEXT("Message \"%s\" should be displayed"),
				*CurrentMessage.Message.ToString()
			);
			Execute_SetMessage(MessageWidget, CurrentMessage);
		}
	}

	if (CurrentMessage.Message.IsEmpty())
	{
		Execute_OnEndDisplayMessage(this);
	}
}

void AMessageableHUD::SetMessage_Implementation(const FNFlashMessage& NewMessage)
{
	FNFlashMessage MyNewMsg = NewMessage;

	// This to avoid eternal message
	if (MyNewMsg.TimeDuration == 0)
	{
		MyNewMsg.TimeDuration = MaxTimeDisplayed;
	}

	MessagesQueues.Add(MyNewMsg);
	if (CurrentMessage.Message.IsEmpty())
	{
		WhenEndDisplayMessage();
	}
}

void AMessageableHUD::ClearMessage_Implementation()
{
	if (MessageWidget != nullptr)
	{
		Execute_ClearMessage(MessageWidget);
	}
	MessagesQueues.Clear();
}

FNFlashMessage AMessageableHUD::GetMessage_Implementation() const
{
	return CurrentMessage;
}

FText AMessageableHUD::GetMessageTxt()
{
	return CurrentMessage.Message;
}

void AMessageableHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMessageableHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	MessageWidget->OnNativeEndDisplayMessage().RemoveAll(this);
	MessageWidget->RemoveFromViewport();
	MessageWidget = nullptr;
}


// BEGIN BP delegates management implementation for OnEndDisplayMessage
FFlashMessageEvent& AMessageableHUD::OnNativeEndDisplayMessage()
{
	return OnEndDisplayMessageEvent;
}

void AMessageableHUD::OnEndDisplayMessage_Implementation()
{
	MessagesQueues.Clear();
	OnEndDisplayMessageEvent.Broadcast();
}

FBPDelegateHandle AMessageableHUD::BindToOnEndDisplayMessage_Implementation(
	const FEventForFlashMessageEvent& Functor)
{
	auto Delegate = TDelegate<void()>::CreateLambda(
		[Functor]()
		{
			Functor.ExecuteIfBound();
		}
	);
	return FBPDelegateHandle(OnEndDisplayMessageEvent.Add(Delegate));
}

void AMessageableHUD::UnbindToOnEndDisplayMessage_Implementation(FBPDelegateHandle& Handle)
{
	if (Handle.RealHandle.IsValid())
	{
		OnEndDisplayMessageEvent.Remove(Handle.RealHandle);
		Handle.RealHandle.Reset();
	}
}

// END BP delegates management implementation for OnEndDisplayMessage
