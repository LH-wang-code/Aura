// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayAuraWidgetController.h"
#include "Ability/AuraAttributeSet.h"

void UOverlayAuraWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());


}
