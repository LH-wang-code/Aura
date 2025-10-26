// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraGameplayTags.h"
#include "AttributeInfo.generated.h"


USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AttributeTag=FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AtttributeValue=0.f;

};
/**
 * 
 */
UCLASS()
class RPGGAME_AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()


public:

	FAuraAttributeInfo  FindAttributeUInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false)const;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)

	TArray<FAuraAttributeInfo> AttributeInfomation;
	
};
