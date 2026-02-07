// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()




public:
	UPROPERTY(EditAnywhere,Category="Summon")
	float MinSpread = 90.f;

	UPROPERTY(EditAnywhere, Category = "Summon")
	int MinionCount = 5;

	UPROPERTY(EditAnywhere, Category = "Summon")
	float MinDistance=50.f;

	UPROPERTY(EditAnywhere, Category = "Summon")
	float MaxDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "Summon")
	TArray<TSubclassOf<APawn>>MinionClasses;

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetActorSpawnLocation();

	UFUNCTION(BlueprintCallable)
	TSubclassOf<APawn> GetRandomMinionClass();

};
