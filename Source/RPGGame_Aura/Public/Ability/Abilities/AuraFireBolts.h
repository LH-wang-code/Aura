// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolts.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_AURA_API UAuraFireBolts : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level)override;
	virtual FString GetNextLevelDescription(int32 Level)override;
	UFUNCTION(BlueprintCallable, Category = "Projectile")

	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingActor);
protected:

	UPROPERTY(EditDefaultsOnly,Category="FireBolt")
	float ProjectileSpread=90.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	int32 MaxNumProjectiles = 5;


	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMin=1600.f ;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")	
	bool bLaunchHomingProjectiles = true;
};
