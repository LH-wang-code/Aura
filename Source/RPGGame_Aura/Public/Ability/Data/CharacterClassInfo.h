// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Marrier,
	Ranger
};

USTRUCT(BlueprintType)
struct  FCharacterClassDefaultInfo
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly,Category="Default Class")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};

/**
 * 
 */
UCLASS()
class RPGGAME_AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()



public:


	UPROPERTY(EditDefaultsOnly, Category = "Character Default Class")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly,Category="Common Default Class")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Default Class")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Default Class")
	TArray<TSubclassOf<UGameplayAbility>>CommonAbilities;

	FCharacterClassDefaultInfo GetInfoWithECharacterClass(ECharacterClass CharacterClass);
	
};
