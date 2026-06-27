// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Abilities/AuraDamageGameplayAbility.h"
#include "GameplayAbilities/Public/AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "AuraGameplayTags.h"
void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

	const float ScalaDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScalaDamage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}

FTaggedMontageInfo UAuraDamageGameplayAbility::GetRandomMontageFromInfoArray(const TArray<FTaggedMontageInfo>& MontageInfoArray)
{
	if (MontageInfoArray.Num() == 0)
	{
		return FTaggedMontageInfo();
	}
	FTaggedMontageInfo MontageInfo = MontageInfoArray[FMath::RandRange(0, MontageInfoArray.Num() - 1)];
	return MontageInfo;
}
