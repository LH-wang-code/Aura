// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP)
{
	int32 level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		if (LevelUpInfo.Num() - 1 <= level)return level;//说明是最高等级

		if (XP >= LevelUpInfo[level].LevelUpRequirement)
		{
			level++;
		}
		else
		{
			bSearching = false;
		}
	}
	return level;
}
