// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Ability/Data/CharacterClassInfo.h" 
#include "AuraAbilityTypes.h"
#include "AuraAbilitySystemFunctionLibrary.generated.h"
 
class UGameplayStatics;
class UOverlayAuraWidgetController;

/**
 * 
 */
UCLASS()
class RPGGAME_AURA_API UAuraAbilitySystemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary| WidgetController")
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject,FWidgetControllerParams&OutWCParams,AAuraHUD*& OutAuraHUD);
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary| WidgetController",meta=(DefaultToSelf="WorldContextObject"))
	static UOverlayAuraWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary| WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenyWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);


	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary| WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary| CharacterClassInfo")
	static void InitializeDefaultCharacterClassInfo(const UObject* WorldContextObject,ECharacterClass CharacterClass, float level,UAbilitySystemComponent* ASC);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary| CharacterClassInfo")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,ECharacterClass CharacterClass);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary| CharacterClassInfo")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary| AbilityInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static bool IsBlockedHit(FGameplayEffectContextHandle& GameplayEffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static bool IsCriticalHit(FGameplayEffectContextHandle& GameplayEffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static bool IsSuccessfulDebuff(FGameplayEffectContextHandle& GameplayEffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static float GetDebuffDamage(FGameplayEffectContextHandle& GameplayEffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static float GetDebuffDuration(FGameplayEffectContextHandle& GameplayEffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static float GetDebuffFrequency(FGameplayEffectContextHandle& GameplayEffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySysteomLibrary | GameplayEffects")

	static FGameplayTag GetDamageType(FGameplayEffectContextHandle& GameplayEffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySysteomLibrary | GameplayEffects")

	static FVector GetDeathImpulse(FGameplayEffectContextHandle& GameplayEffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySysteomLibrary | GameplayEffects")

	static FVector GetKnockbackForce(FGameplayEffectContextHandle& GameplayEffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref)FGameplayEffectContextHandle& GameplayEffectContextHandle, bool bInIsBlockedHit);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& GameplayEffectContextHandle, bool bInIsCriticalHit);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static void SetIsSuccessfulDebuff(UPARAM(ref)FGameplayEffectContextHandle& GameplayEffectContextHandle, bool bInIsSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref)FGameplayEffectContextHandle& GameplayEffectContextHandle, float InDebuffDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& GameplayEffectContextHandle, float InDebuffDuration);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& GameplayEffectContextHandle, float InDebuffFrequency);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static void SetDamageType(UPARAM(ref)FGameplayEffectContextHandle& GameplayEffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref)FGameplayEffectContextHandle& GameplayEffectContextHandle, const FVector& InDeathImpulse);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref)FGameplayEffectContextHandle& GameplayEffectContextHandle, const FVector& InKnockbackForce);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | GameplayMachanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldObjectContext,TArray<AActor*>& OutOverlappingActors,TArray<AActor*> ActorsToIgnore,float Radius,const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | GameplayMachanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | GameplayMachanics")
	static int32 GetXPRewardForCharacterClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 level);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | DamageEffect")
	static FGameplayEffectContextHandle ApplyGameplayEffect(FDamageEffectParams Params);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | DamageEffect")

	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis,float Spread, int32 NumsRotators);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySysteomLibrary | DamageEffect")

	static TArray<FVector>EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread,int32 NumsVectors);
};
