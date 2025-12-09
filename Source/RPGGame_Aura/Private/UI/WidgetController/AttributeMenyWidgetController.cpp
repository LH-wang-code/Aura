// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenyWidgetController.h"
#include "Ability/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "Ability/Data/AttributeInfo.h"

void UAttributeMenyWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);
	//FAuraAttributeInfo Info = AttributeInfo->FindAttributeUInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength, true);
	//const FGameplayTag StrengthTag = FAuraGameplayTags::Get().Attributes_Primary_Strength;
	//UE_LOG(LogTemp, Warning, TEXT("StrengthTag: %s"), *StrengthTag.ToString());
	//Info.AtttributeValue = AS->GetStrength();
	//AttributeInfoDelegate.Broadcast(Info); 

	for (auto& Pair : AS->TagsToAttributes)
	{

		BroadcastAttributeInfo(Pair.Key,Pair.Value());
		
	}
	for (const auto& Pair : AS->TagsToAttributes)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeUInfoForTag(Pair.Key);
		if (Info.AttributeName.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("Missing AttributeInfo for Tag: %s"), *Pair.Key.ToString());
		}
	}

}
void UAttributeMenyWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binding: %s �� %s"), *Pair.Key.ToSt            ring(), *Pair.Value().GetName());

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair](const FOnAttributeChangeData& Data)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Broadcasting: %s �� %f"),
				//	*Pair.Key.ToString(), Pair.Value().GetNumericValue(AS));

				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}


}

void UAttributeMenyWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,const FGameplayAttribute& Attribute)const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeUInfoForTag(AttributeTag);
	Info.AtttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
