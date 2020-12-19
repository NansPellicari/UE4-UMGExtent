// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Messageable.h"
#include "GameFramework/HUD.h"
#include "Containers/Queue.h"
#include "Internationalization/Text.h"
#include "NansUE4Utilities/Public/MessageTypes.h"
#include "MessageableHUD.generated.h"

class UMessageableWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNansPlayerHUDDisplayEvent, const FNFlashMessage, Message);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNansPlayerHUDStopDisplayEvent);

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class NANSUMGEXTENT_API AMessageableHUD : public AHUD, public INMessageable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Message")
	bool bDebug = false;

	UFUNCTION()
	void WhenEndDisplayMessage();

	/**
	 * Set a message to the player.
	 */
	virtual void SetMessage_Implementation(const FNFlashMessage& NewMessage) override;
	virtual void ClearMessage_Implementation() override;
	virtual FNFlashMessage GetMessage_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "Message")
	FText GetMessageTxt();

	// BEGIN BP delegates management declaration for OnEndDisplayMessage
	FFlashMessageEvent OnEndDisplayMessageEvent;
	virtual FFlashMessageEvent& OnNativeEndDisplayMessage() override;
	virtual void OnEndDisplayMessage_Implementation() override;
	virtual FBPDelegateHandle BindToOnEndDisplayMessage_Implementation(
		const FEventForFlashMessageEvent& Functor) override;
	virtual void UnbindToOnEndDisplayMessage_Implementation(FBPDelegateHandle& Handle) override;
	// END BP delegates management declaration for OnEndDisplayMessage

	virtual void Tick(float DeltaSeconds);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


protected:
	AMessageableHUD();

	/** The class of User Widget to create and use for display message */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
	TSubclassOf<UMessageableWidget> MessageWidgetClass;

	/** The User Widget object displayed and managed by this HUD to display message */
	UPROPERTY(Transient, DuplicateTransient, BlueprintReadWrite, Category = "Message")
	UMessageableWidget* MessageWidget;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Disable any message after this time (to avoid eternal message)
	float MaxTimeDisplayed = 100.f;
	FNFlashMessageList MessagesQueues;
	FNFlashMessage CurrentMessage;
};
