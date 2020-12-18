// Fill out your copyright notice in the Description page of Project Settings.

#include "NansUMGExtent//Public/Components/NansWidgetComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "NansUMGExtent/Public/Blueprint/NansUserWidget.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"
#include "Slate/SceneViewport.h"


void UNansWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // No needs to interact if not visible
    if (!IsVisible()) return;

    if (GetWorld() != nullptr && GetWorld()->IsGameWorld()) {
        ManageTouchEvents();
    }
}

void UNansWidgetComponent::ManageTouchEvents()
{
    APlayerController* Controller = GetWorld()->GetFirstPlayerController();

    if (Widget == nullptr) return;
    if (Controller == nullptr) return;
    if (!GEngine || !GEngine->GameViewport || !GEngine->GameViewport->GetGameViewport()) return;

    float TouchX, TouchY;
    bool bIsCurrentlyPressed;
    Controller->GetInputTouchState(ETouchIndex::Touch1, TouchX, TouchY, bIsCurrentlyPressed);

    if (!bIsCurrentlyPressed && bWidgetIsTouched) {
        bWidgetIsTouched = false;
        UE_LOG(LogTemp, Warning, TEXT("Touch End"));
        
        // Final goal is here
        // DispatchOnInputTouchEnd(ETouchIndex::Touch1);
    }

    if (!bIsCurrentlyPressed) return;

    PrevTouchPos = TouchPos;

    TouchPos.X = TouchX;
    TouchPos.Y = TouchY;

    // No interaction here!
    if (Widget->GetVisibility() != ESlateVisibility::SelfHitTestInvisible
        && Widget->GetVisibility() != ESlateVisibility::Visible) {
        return;
    }

    if (!InWidgetZone(TouchPos, Widget->GetRootWidget())) return;

    if (!bWidgetIsTouched) {
        bWidgetIsTouched = true;
        UE_LOG(LogTemp, Warning, TEXT("Touch Start"));
        // Final goal is here
        //DispatchOnInputTouchBegin(ETouchIndex::Touch1);
        return;
    }

    if (!PrevTouchPos.Equals(TouchPos, 0.0001f)) {
        UE_LOG(LogTemp, Warning, TEXT("Touch Move"));
    }
}

bool UNansWidgetComponent::InWidgetZone(FVector2D UserTouchPos, UWidget * WidgetToCheck)
{
    // No need to go further here
    if (WidgetToCheck->GetVisibility() != ESlateVisibility::SelfHitTestInvisible
        && WidgetToCheck->GetVisibility() != ESlateVisibility::Visible) {
        return false;
    }

    // Check if in zone
    if (WidgetToCheck->GetVisibility() == ESlateVisibility::Visible) {
        FGeometry ViewportGeo = GEngine->GameViewport->GetGameViewport()->GetCachedGeometry();
        // This works only for a visible widget.
        FGeometry Geo = WidgetToCheck->GetCachedGeometry();
        FVector2D WidgetSize = Geo.GetAbsoluteSize();
        FVector2D WidgetPos = Geo.GetAbsolutePositionAtCoordinates(FVector2D(0, 0));
        WidgetPos = ViewportGeo.AbsoluteToLocal(WidgetPos);

        // Check if touch position is in the widget zone
        if (UserTouchPos > WidgetPos && UserTouchPos < WidgetPos + WidgetSize) {
            return true;
        }
    }

    // If touch position is not on the widget or his visibility is set to SelfHitTestInvisible,
    // maybe we have better chance in it's children ?

    TArray<UWidget*> AllWidgets;
    // Retrieve each direct children to test them
    UNansWidgetComponent::ForWidgetAndChildren(Widget, [&AllWidgets](UWidget* ChildWidget) {
        AllWidgets.Add(ChildWidget);
    }, false);

    for (UWidget* SubWidget : AllWidgets)
    {
        // Go to check recursively in this widget's children
        bool bIsInZone = InWidgetZone(TouchPos, SubWidget);

        // If a child is hit, no need to go further
        if (bIsInZone) {
            return true;
        }
        else {
            continue;
        }
    }

    return false;
}

void UNansWidgetComponent::ForWidgetAndChildren(UWidget * Widget, TFunctionRef<void(UWidget*)> Predicate, bool Recursive)
{
    // Search for any named slot with content that we need to dive into.
    if (INamedSlotInterface* NamedSlotHost = Cast<INamedSlotInterface>(Widget))
    {
        TArray<FName> SlotNames;
        NamedSlotHost->GetSlotNames(SlotNames);

        for (FName SlotName : SlotNames)
        {
            if (UWidget* SlotContent = NamedSlotHost->GetContentForSlot(SlotName))
            {
                Predicate(SlotContent);

                if (Recursive) {
                    ForWidgetAndChildren(SlotContent, Predicate);
                }
            }
        }
    }

    // Search standard children.
    if (UPanelWidget* PanelParent = Cast<UPanelWidget>(Widget))
    {
        for (int32 ChildIndex = 0; ChildIndex < PanelParent->GetChildrenCount(); ChildIndex++)
        {
            if (UWidget* ChildWidget = PanelParent->GetChildAt(ChildIndex))
            {
                Predicate(ChildWidget);

                if (Recursive) {
                    ForWidgetAndChildren(ChildWidget, Predicate);
                }
            }
        }
    }
}

void UNansWidgetComponent::OnVisibilityChanged()
{
    Super::OnVisibilityChanged();

    if (!IsVisible()) {
        bWidgetIsTouched = false;
    }
}
