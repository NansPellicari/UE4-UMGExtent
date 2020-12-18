// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "NansWidgetComponent.generated.h"

/**
 *
 */
UCLASS(Blueprintable, ClassGroup = ("UserInterface"), meta = (BlueprintSpawnableComponent))
class NANSUMGEXTENT_API UNansWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	// This is pasted from UWidgetTree::ForWidgetAndChildren function,
	// But here it allows to go recursively or not
	static void ForWidgetAndChildren(UWidget* Widget, TFunctionRef<void(UWidget*)> Predicate, bool Recursive = true);

	UFUNCTION(BlueprintCallable, Category = "UserInterface")
	EWidgetSpace MyGetWidgetSpace() const { return Space; }

protected:
	virtual void OnVisibilityChanged();

private:
	// This vars are dedicated to let Touch Events work on this Comp.
	// for more details, see https://answers.unrealengine.com/questions/896176/does-anyone-have-a-solution-to-make-widgetcomponen.html?childToView=896296
	bool bWidgetIsTouched = false;
	FVector2D PrevTouchPos;
	FVector2D TouchPos;
	void ManageTouchEvents();
	bool InWidgetZone(FVector2D UserTouchPos, UWidget* Widget);
};
