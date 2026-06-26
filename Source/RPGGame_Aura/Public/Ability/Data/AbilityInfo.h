// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

/**
 * 存取UI部分的能力信息
 */
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAuraAbilityInfo 
{

	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag=FGameplayTag();
	UPROPERTY(BlueprintReadOnly)

	FGameplayTag StatusTag = FGameplayTag();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InCooldownTag=FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D>Icon=nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface>BackgroundMaterial=nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement=1;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility>Ability;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)

	FGameplayTag AbilityType = FGameplayTag();

};
UCLASS()
class RPGGAME_AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Ability Infomation")
	TArray<FAuraAbilityInfo>AuraAbilityInfo;

	FAuraAbilityInfo FindAbilityInfoForTag(const FGameplayTag& Tag,bool bLogNotFound=false);

};
