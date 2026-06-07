// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Data/AbilityInfo.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& Tag, bool bLogNotFound)
{
	for (const FAuraAbilityInfo& Info : AuraAbilityInfo)
	{
		if (Info.AbilityTag == Tag)
		{
			bLogNotFound = true;
			return Info;
		}
	}
	bLogNotFound = false;

	return FAuraAbilityInfo();
}
