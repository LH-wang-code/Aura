// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include <../../../../../../../Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h>
#include "SpellMenuWidgetController.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled,FString,Description,FString,NextDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityTag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassigned, const FGameplayTag&, AbilityTag);


/**
 * 
 */
struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

UCLASS(BlueprintType, Blueprintable)
class RPGGAME_AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:

	FSelectedAbility SelectedAbility;
	virtual void BroadcastInitialValues()override;
	virtual void BindCallbacksToDependencies()override;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Spell")
	FOnPlayerStateChangedSignature OnSpellPointsChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;
	UPROPERTY(BlueprintAssignable)

	FWaitForEquipSelectionSignature WaitForEquipDelegate;

	UPROPERTY(BlueprintAssignable)

	FWaitForEquipSelectionSignature StopWaitForEquipDelegate;
	UPROPERTY(BlueprintAssignable)

	FSpellGlobeReassigned SpellGlbeReassignedDelegate;
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpellGlobeDeSelected();

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();
	UFUNCTION(BlueprintCallable)

	void EquipButtonPressed();

	UFUNCTION(BlueprintCallable)

	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);


	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot);

private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointButton, bool& bShouldEnableEquipButton);

	int32 CurrentSpellPoints = 0;


	bool bWaitingForEquipping=false;


	//当前装备的法术所在插槽
	FGameplayTag SelectedSlot;

};
