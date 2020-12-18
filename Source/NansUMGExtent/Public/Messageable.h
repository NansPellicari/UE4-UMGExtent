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
#include "UObject/Interface.h"
#include "MessageTypes.h"
#include "Misc/BPDelegateHandle.h"

#include "Messageable.generated.h"

DECLARE_MULTICAST_DELEGATE(FFlashMessageEvent);
DECLARE_DYNAMIC_DELEGATE(FEventForFlashMessageEvent);


UINTERFACE(MinimalAPI, BlueprintType)
class UNMessageable : public UInterface
{
	GENERATED_BODY()
};

class NANSUMGEXTENT_API INMessageable
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Message")
	void SetMessage(const FNFlashMessage& NewMessage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Message")
	void ClearMessage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Message")
	FNFlashMessage GetMessage() const;

	// BEGIN BP delegates management declaration for OnEndDisplayMessage
	UFUNCTION(BlueprintNativeEvent, Category = "Message")
	void OnEndDisplayMessage();

	virtual FFlashMessageEvent& OnNativeEndDisplayMessage() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Message")
	FBPDelegateHandle BindToOnEndDisplayMessage(const FEventForFlashMessageEvent& Functor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Message")
	void UnbindToOnEndDisplayMessage(UPARAM(ref) FBPDelegateHandle& Handle);
	// END BP delegates management declaration for OnEndDisplayMessage
};
