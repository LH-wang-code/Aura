// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Abilities/AuraDamageGameplayAbility.h"
#include "GameplayAbilities/Public/AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (TTuple<FGameplayTag, FScalableFloat>Pair : DamageTypes)
	{
		const float ScalaDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScalaDamage);

	}
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

