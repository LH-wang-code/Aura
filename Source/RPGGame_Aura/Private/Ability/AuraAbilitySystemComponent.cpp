// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Ability/Abilities/AuraGameplayAbility.h"
//当AbilitySystemComponent的Actor信息设置完成时调用该函数
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// 1. 注册效果应用委托
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
	// 2. 获取游戏标签
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,1);
		if (const UAuraGameplayAbility* AuraAbility=Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return ;

	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return ;

	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
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
