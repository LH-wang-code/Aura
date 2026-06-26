// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include <../../../../../../../Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h>
#include "Ability/Data/AbilityInfo.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Player/AuraPlayerState.h"
#include "AuraGameplayTags.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	if (SelectedAbility.Ability.IsValid() == false)
	{
		const auto& Tags = FAuraGameplayTags::Get();
		SelectedAbility.Ability = Tags.Abilities_None;
		SelectedAbility.Status = Tags.Abilities_Status_Locked;
	}

	BroadcastAbilityInfo();
	OnSpellPointsChangedDelegate.Broadcast(GetPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{



	GetAuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag,int32 NewLevel)
		{

			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;

				bool bEnableSpendPoints = false;
				bool bEnableEquip = false;


				ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
				FString Description;
				FString NextDescription;

				GetAuraASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextDescription);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip,Description,NextDescription);
			}

			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoSignature.Broadcast(Info);
			}
		});

	GetPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 SpellPoints)
		{
			OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;


			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			FString Description;
			FString NextDescription;

			GetAuraASC()->GetDescriptionByAbilityTag(SelectedAbility.Ability, Description, NextDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextDescription);
		}

	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{


	if (bWaitingForEquipping)
	{
		FGameplayTag SelectedAbiityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitForEquipDelegate.Broadcast(SelectedAbiityType);
	}

	const int32 SpellPoints=GetPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	bool bTagValid = AbilityTag.IsValid();
	bool bTagNone = AbilityTag.MatchesTag(FAuraGameplayTags::Get().Abilities_None);

	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);

	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid||bTagNone||!bSpecValid)
	{
		AbilityStatus = FAuraGameplayTags::Get().Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);

	FString Description;
	FString NextDescription;
	GetAuraASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextDescription);
}

void USpellMenuWidgetController::SpellGlobeDeSelected()
{

	if (bWaitingForEquipping)
	{
		FGameplayTag SelectedAbiityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitForEquipDelegate.Broadcast(SelectedAbiityType);
		bWaitingForEquipping = false;
	}


	//制成默认值
	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;
	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());

}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServeSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType= AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipping = true;
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	bShouldEnableSpellPointButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointButton=true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnableEquipButton = false;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointButton = true;
		}
	}
	else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Locked))
	{
		bShouldEnableEquipButton = false;

	}
	
}
