// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Ability/Data/CharacterClassInfo.h" 
#include "AuraAbilitySystemFunctionLibrary.generated.h"
 

class UOverlayAuraWidgetController;

/**
 * 
 */
UCLASS()
class RPGGAME_AURA_API UAuraAbilitySystemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary| WidgetController")
	static UOverlayAuraWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary| WidgetController")
	static UAttributeMenyWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary| CharacterClassInfo")
	static void InitializeDefaultCharacterClassInfo(const UObject* WorldContextObject,ECharacterClass CharacterClass, float level,UAbilitySystemComponent* ASC);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary| CharacterClassInfo")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

};
