//  Copyright 2020-present Nans Pellicari (nans.pellicari@gmail.com).
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

#include "Widget/MessageableWidget.h"


void UMessageableWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (MessageDurationElapsed > 0)
	{
		MessageDurationElapsed -= InDeltaTime;

		if (MessageDurationElapsed <= 0)
		{
			MessageDurationElapsed = 0;
			Execute_OnEndDisplayMessage(this);
			Execute_ClearMessage(this);
		}
	}
}

void UMessageableWidget::SetMessageDuration(float Duration)
{
	MessageDurationElapsed = Duration;
}

// BEGIN BP delegates management implementation for OnEndDisplayMessage
FFlashMessageEvent& UMessageableWidget::OnNativeEndDisplayMessage()
{
	return OnEndDisplayMessageEvent;
}

void UMessageableWidget::OnEndDisplayMessage_Implementation()
{
	OnEndDisplayMessageEvent.Broadcast();
}

FBPDelegateHandle UMessageableWidget::BindToOnEndDisplayMessage_Implementation(
	const FEventForFlashMessageEvent& Functor)
{
	auto Delegate = TBaseDelegate<void>::CreateLambda(
		[Functor]()
		{
			Functor.ExecuteIfBound();
		}
	);
	return FBPDelegateHandle(OnEndDisplayMessageEvent.Add(Delegate));
}

void UMessageableWidget::UnbindToOnEndDisplayMessage_Implementation(FBPDelegateHandle& Handle)
{
	if (Handle.RealHandle.IsValid())
	{
		OnEndDisplayMessageEvent.Remove(Handle.RealHandle);
		Handle.RealHandle.Reset();
	}
}

// END BP delegates management implementation for OnEndDisplayMessage
