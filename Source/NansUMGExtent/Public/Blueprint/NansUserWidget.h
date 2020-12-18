// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "NansUserWidget.generated.h"

struct FGeometry;
struct FPointerEvent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNansUserWidgetEvent, UWidget*, Widget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNansUserWidgetInteractionEvent, const FGeometry&, InGeometry, const FPointerEvent&, InEvent);

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class NANSUMGEXTENT_API UNansUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    FNansUserWidgetInteractionEvent DOnMouseButtonDown;
    FNansUserWidgetInteractionEvent DOnMouseButtonUp;
    FNansUserWidgetInteractionEvent DOnTouchStarted;
    FNansUserWidgetInteractionEvent DOnTouchEnded;

    UPROPERTY(BlueprintAssignable, Category = "Event")
    FNansUserWidgetEvent OnAddedChild;

    UPROPERTY(BlueprintAssignable, Category = "Event")
    FNansUserWidgetEvent OnRemovedChild;

    UFUNCTION(BlueprintCallable, Category = "Widget")
        UWidgetTree* GetWidgetTree() const;

    UFUNCTION(BlueprintCallable, Category = "Widget")
        UWidget* FindWidget(const FName& Name) const;

    UFUNCTION(BlueprintNativeEvent)
        void Reset();
    virtual void Reset_Implementation();

    UFUNCTION(BlueprintCallable)
        void CallReset();


protected:
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
    virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;


};
