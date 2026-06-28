// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Abilities/AuraGameplayAbility.h"
#include "AuraAbilityTypes.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
struct FTaggedMontageInfo;
UCLASS()
class RPGGAME_AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect>DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat>DamageTypes;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")

	float DebuffChance = 20.f;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")

	float DebuffDamage = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")

	float DebuffFrequency = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")

	float DebuffDuration = 5.f;

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FTaggedMontageInfo GetRandomMontageFromInfoArray(const TArray<FTaggedMontageInfo>& MontageInfoArray);

	FDamageEffectParams MakeDamageEffectParamsFromClass(AActor* TargetActor=nullptr);

};
