// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"


class UAuraUserWidget;
class UOverlayAuraWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class RPGGAME_AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
	


public:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget>OverlayWidget;


	UOverlayAuraWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);



private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget>OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayAuraWidgetController>OverlayAuraWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayAuraWidgetController>OverlayWidgetControllerClass;
};
