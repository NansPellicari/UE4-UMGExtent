// Fill out your copyright notice in the Description page of Project Settings.

#include "NansUMGExtent/Public/Blueprint/NansUserWidget.h"

#include "Runtime/UMG/Public/Blueprint/WidgetTree.h"
#include "SlateCore/Public/Input/Events.h"
#include "SlateCore/Public/Layout/Geometry.h"

UWidgetTree* UNansUserWidget::GetWidgetTree() const
{
	return WidgetTree;
}

UWidget* UNansUserWidget::FindWidget(const FName& Name) const
{
	return WidgetTree->FindWidget(Name);
}

void UNansUserWidget::CallReset()
{
	Reset();
	Reset_Implementation();
}

void UNansUserWidget::Reset_Implementation() {}

FReply UNansUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	DOnMouseButtonDown.Broadcast(InGeometry, InMouseEvent);
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UNansUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	DOnMouseButtonUp.Broadcast(InGeometry, InMouseEvent);
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UNansUserWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	DOnTouchStarted.Broadcast(InGeometry, InGestureEvent);
	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UNansUserWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	DOnTouchEnded.Broadcast(InGeometry, InGestureEvent);
	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
}
