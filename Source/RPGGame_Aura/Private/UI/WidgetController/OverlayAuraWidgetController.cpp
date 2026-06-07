// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayAuraWidgetController.h"
#include "Ability/AuraAttributeSet.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

void UOverlayAuraWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayAuraWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
			
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) 
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);

		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (AuraASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(AuraASC);
		}
		else
		{
			AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayAuraWidgetController::OnInitializeStartupAbilities);
		}

		AuraASC->EffectAssetTags.AddLambda(

			[this](const FGameplayTagContainer& AssetTags)
			{
				for (FGameplayTag Tag : AssetTags)
				{

					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{

						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						if (!Row)return;
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}

	//GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied!"));


}

void UOverlayAuraWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC)
{
	if (!AuraASC->bStartupAbilitiesGiven)return;


	FForEachAbility BroadcastDelegate;
	//通过tag和spec找到相关的ability信息并广播出去。创建委托之后，在foreachability中遍历时，会激活该匿名函数
	//BroadcastDelegate.BindLambda([this, AuraASC](const FGameplayAbilitySpec& AbilitySpec)
	//	{
	//		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuraASC->GetAbilityTagFromSpec(AbilitySpec));

	//		Info.InputTag = AuraASC->GetInputTagFromSpec(AbilitySpec);
	//		AbilityInfoSignature.Broadcast(Info);
	//	});
	//使用静态方法替换lambda
	AuraASC->ForEachAbility(FForEachAbility::CreateStatic(&ProcessAbilityStatic,this,AuraASC));
}

void UOverlayAuraWidgetController::ProcessAbilityStatic(const FGameplayAbilitySpec& AbilitySpec, UOverlayAuraWidgetController* Controller, UAuraAbilitySystemComponent* AuraASC)
{
	if (!Controller || !AuraASC || !Controller->AbilityInfo)
	{
		return ;
	}
	FAuraAbilityInfo Info = Controller->AbilityInfo->FindAbilityInfoForTag(AuraASC->GetAbilityTagFromSpec(AbilitySpec));
	Info.InputTag = AuraASC->GetInputTagFromSpec(AbilitySpec);
	Controller->AbilityInfoSignature.Broadcast(Info);
	return ;
}
