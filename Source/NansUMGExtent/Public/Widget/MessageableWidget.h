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

#pragma once

#include "CoreMinimal.h"
#include "NansUMGExtent/Public/Blueprint/NansUserWidget.h"
#include "Messageable.h"

#include "MessageableWidget.generated.h"

/**
 *
 */
UCLASS(Abstract)
class NANSUMGEXTENT_API UMessageableWidget : public UNansUserWidget, public INMessageable
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Message")
	void SetMessageDuration(float Duration);

	// BEGIN BP delegates management declaration for OnEndDisplayMessage
	FFlashMessageEvent OnEndDisplayMessageEvent;
	virtual FFlashMessageEvent& OnNativeEndDisplayMessage() override;
	virtual void OnEndDisplayMessage_Implementation() override;
	virtual FBPDelegateHandle BindToOnEndDisplayMessage_Implementation(
		const FEventForFlashMessageEvent& Functor) override;
	virtual void UnbindToOnEndDisplayMessage_Implementation(FBPDelegateHandle& Handle) override;
	// END BP delegates management declaration for OnEndDisplayMessage

private:
	float MessageDurationElapsed = 5.f;
};
