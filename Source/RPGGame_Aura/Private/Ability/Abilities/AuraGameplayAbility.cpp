// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Abilities/AuraGameplayAbility.h"
#include "Ability/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s,</><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum  LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum",Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s,</><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum  LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s,</><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum  LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);

}

float UAuraGameplayAbility::GetManaCost(float Inlevel /*= 1.f*/)const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (auto Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(Inlevel,ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float InLevel /*= 1.f*/)
{
	float CoolDown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, CoolDown);
	}
	return CoolDown;
}
