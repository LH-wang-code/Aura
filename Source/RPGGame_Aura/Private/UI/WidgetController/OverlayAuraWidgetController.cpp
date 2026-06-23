// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayAuraWidgetController.h"
#include "Ability/AuraAttributeSet.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Player/AuraPlayerState.h"

void UOverlayAuraWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAS()->GetMaxMana());
}

void UOverlayAuraWidgetController::BindCallbacksToDependencies()
{
	//经验及升级相关
	
	GetPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayAuraWidgetController::OnXPChanged);
	//AuraPlayerState->OnLevelChangedDelegate.AddUObject(this, &UOverlayAuraWidgetController::OnLevelChanged);

	GetPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}

	);

	//属性广播

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAS()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
			
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAS()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) 
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);

		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAS()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAS()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	if (GetAuraASC())
	{
		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayAuraWidgetController::BroadcastAbilityInfo);
		}

		GetAuraASC()->EffectAssetTags.AddLambda(

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
	GetWorld()->GetTimerManager().SetTimerForNextTick(
		this,
		&UOverlayAuraWidgetController::BroadcastAbilityInfo
	);
	UE_LOG(LogTemp, Warning, TEXT("Ability Num = %d"),
		GetAuraASC()->GetActivatableAbilities().Num());
}

void UOverlayAuraWidgetController::OnXPChanged(int32 InXP)
{

	if (ULevelUpInfo* LevelUpInfo = GetPS()->LevelUpInfo)
	{
		int level = LevelUpInfo->FindLevelForXP(InXP);
		int MaxLevel = LevelUpInfo->LevelUpInfo.Num();
		if (MaxLevel >= level && level > 0)
		{
			//对应等级所需经验
			int32 LevelUpRequirement = LevelUpInfo->LevelUpInfo[level].LevelUpRequirement;
			int32 preLevelUpRequirement= LevelUpInfo->LevelUpInfo[level-1].LevelUpRequirement;
			int32 DelLevelUpRequirement = InXP - preLevelUpRequirement;
			float value = static_cast<float>(DelLevelUpRequirement)  / static_cast<float>(LevelUpRequirement);
			UE_LOG(LogTemp, Warning, TEXT("LevelUpRequirement:%d,preLevelUpRequirement:%d,value:&f"), LevelUpRequirement, preLevelUpRequirement, value);
			OnXPPercentChangedSignature.Broadcast(value);
		}
	}

}

void UOverlayAuraWidgetController::OnLevelChanged(int32 InLevel)
{

}

//void UOverlayAuraWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC)
//{
//	if (!GetAuraASC()->bStartupAbilitiesGiven)return;
//
//
//	FForEachAbility BroadcastDelegate;
//	//通过tag和spec找到相关的ability信息并广播出去。创建委托之后，在foreachability中遍历时，会激活该匿名函数
//	//BroadcastDelegate.BindLambda([this, AuraASC](const FGameplayAbilitySpec& AbilitySpec)
//	//	{
//	//		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuraASC->GetAbilityTagFromSpec(AbilitySpec));
//
//	//		Info.InputTag = AuraASC->GetInputTagFromSpec(AbilitySpec);
//	//		AbilityInfoSignature.Broadcast(Info);
//	//	});
//	//使用静态方法替换lambda
//	GetAuraASC()->ForEachAbility(FForEachAbility::CreateStatic(&ProcessAbilityStatic,this, GetAuraASC()));
//}
//
//void UOverlayAuraWidgetController::ProcessAbilityStatic(const FGameplayAbilitySpec& AbilitySpec, UOverlayAuraWidgetController* Controller, UAuraAbilitySystemComponent* AuraASC)
//{
//	if (!Controller || !AuraASC || !Controller->AbilityInfo)
//	{
//		return ;
//	}
//	FAuraAbilityInfo Info = Controller->AbilityInfo->FindAbilityInfoForTag(AuraASC->GetAbilityTagFromSpec(AbilitySpec));
//	Info.InputTag = AuraASC->GetInputTagFromSpec(AbilitySpec);
//	Controller->AbilityInfoSignature.Broadcast(Info);
//	return ;
//}
