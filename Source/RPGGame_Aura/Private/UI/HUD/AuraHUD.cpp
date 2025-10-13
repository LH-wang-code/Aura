// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widgets/AuraUserWidget.h"

#include "UI/WidgetController/OverlayAuraWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
UOverlayAuraWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayAuraWidgetController == nullptr)
	{
		OverlayAuraWidgetController = NewObject<UOverlayAuraWidgetController>(this, OverlayWidgetControllerClass);
		OverlayAuraWidgetController->SetWidgetControllerParams(WCParams);

	}
	return OverlayAuraWidgetController;

}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{

	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass Uninitialized"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetClass Uninitialized"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	//OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);



	FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);

	UOverlayAuraWidgetController* WidgetController=GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);

	
	WidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}

