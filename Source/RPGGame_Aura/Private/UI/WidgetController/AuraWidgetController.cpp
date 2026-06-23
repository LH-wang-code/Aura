// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Ability/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerController.h"
#include "Ability/Data/AbilityInfo.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;

}

void UAuraWidgetController::BroadcastInitialValues()
{

}

void UAuraWidgetController::BindCallbacksToDependencies()
{

}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraASC()->bStartupAbilitiesGiven)return;
	
	
		FForEachAbility BroadcastDelegate;
		//通过tag和spec找到相关的ability信息并广播出去。创建委托之后，在foreachability中遍历时，会激活该匿名函数
		BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(GetAuraASC()->GetAbilityTagFromSpec(AbilitySpec));
	
				Info.InputTag = GetAuraASC()->GetInputTagFromSpec(AbilitySpec);
				AbilityInfoSignature.Broadcast(Info);
			});
		GetAuraASC()->ForEachAbility(BroadcastDelegate);
}

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (AuraAbilitySystemComponent== nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

AAuraPlayerState* UAuraWidgetController::GetPS()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}

UAuraAttributeSet* UAuraWidgetController::GetAS()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}
