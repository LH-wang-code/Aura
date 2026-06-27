// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Abilities/AuraFireBolts.h"
#include <../../../../../../../Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h>
#include "AuraGameplayTags.h"
FString UAuraFireBolts::GetDescription(int32 Level)
{
	const int32 ScalaDamage = Damage.GetValueAtLevel(Level);

	const float ManaCost = GetManaCost(Level);
	const float cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n"
			"<Small>Level:</><Level>%d</>"
			"<Small>ManaCost:</><ManaCost>%.1f</>"
			"<Small>Cooldown:</><CoolDown>%.1f</>"

			"<Defauta>launches a bolt of fine, exploding on impact and depling: </>"
			"<Damage>< / ><Damage>Fire Damage with a chance to burn< / >"
		""), Level, ManaCost ,cooldown, ScalaDamage);
	}
	else
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n<Defauta>launches %d bolt of fine, exploding on impact and depling: </><Damage></><Damage>Fire Damage with a chance to burn</>\n\n<Small>Level:</><Level>%d</>"), FMath::Min(Level, NumProjectiles), ScalaDamage, Level);

	}
}

FString UAuraFireBolts::GetNextLevelDescription(int32 Level)
{
	const int32 ScalaDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level+1);
	const float cooldown = GetCooldown(Level+1);
	return FString::Printf(TEXT("<Title>FIRE BOLT</>\n"
		"<Small>Level:</><Level>%d</>"
		"<Small>ManaCost:</><ManaCost>%.1f</>"
		"<Small>Cooldown:</><CoolDown>%.1f</>"

		"<Defauta>launches a bolt of fine, exploding on impact and depling: </>"
		"<Damage>< / ><Damage>Fire Damage with a chance to burn< / >"
		""), Level+1, ManaCost, cooldown, ScalaDamage);
}