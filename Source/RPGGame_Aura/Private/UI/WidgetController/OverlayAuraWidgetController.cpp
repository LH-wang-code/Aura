// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayAuraWidgetController.h"
#include "Ability/AuraAttributeSet.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Player/AuraPlayerState.h"

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
	//经验及升级相关
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayAuraWidgetController::OnXPChanged);
	//AuraPlayerState->OnLevelChangedDelegate.AddUObject(this, &UOverlayAuraWidgetController::OnLevelChanged);

	AuraPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}

	);

	//属性广播
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

void UOverlayAuraWidgetController::OnXPChanged(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	if (ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo)
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
