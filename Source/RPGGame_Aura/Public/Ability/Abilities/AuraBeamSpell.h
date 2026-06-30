// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Abilities/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * 
 */
class ACharacter;
UCLASS()
class RPGGAME_AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);


	UFUNCTION(BlueprintCallable)

	void StoreOwnerVaribles();


	UFUNCTION(BlueprintCallable)

	void TraceFirstTarget(const FVector& BeamTargetLocation);

	UFUNCTION(BlueprintCallable)

	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);
protected:

	UPROPERTY(BlueprintReadWrite,Category="Beam")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor>MouseHitActor;


	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController>OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<ACharacter>OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	int32 MaxNumShockTargets = 5;

};
