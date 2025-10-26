// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeUInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound /*= false*/) const
{

	for (const FAuraAttributeInfo& Info : AttributeInfomation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Caan't find Info with Tag:[%s]"),*AttributeTag.ToString());
	}
	return FAuraAttributeInfo();

}
