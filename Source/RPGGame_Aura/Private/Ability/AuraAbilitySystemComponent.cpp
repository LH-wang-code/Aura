// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
//当AbilitySystemComponent的Actor信息设置完成时调用该函数
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// 1. 注册效果应用委托
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
	// 2. 获取游戏标签
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,1);
	//	GiveAbility(AbilitySpec);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
	
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	/*
	AbilitySystemComponent: 应用该效果的能力系统组件

	EffectSpec: 包含效果详细信息的规格对象

	ActiveEffectHandle: 激活效果的句柄，用于后续管理
	
	*/
	//从EffectSpec中获取所有资源标签并填充到容器中
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	//广播获取到的标签容器
	EffectAssetTags.Broadcast(TagContainer);
}
