// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "GameplayTagContainer.h"
#include "Ability/Data/AbilityInfo.h"
#include "OverlayAuraWidgetController.generated.h"

struct FOnAttributeChangeData;
struct FGameplayAbilitySpec;
class UAuraAbilitySystemComponent;
USTRUCT(BlueprintType)
struct FUIWidgetRow :public FTableRowBase
{
	GENERATED_BODY()



	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget>MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;

};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature,const FAuraAbilityInfo&,Info);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXPPercentChangedSignature, float, NewValue);


/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class RPGGAME_AURA_API UOverlayAuraWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()


public:
	virtual void BroadcastInitialValues()override;
	virtual void BindCallbacksToDependencies()override;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FAbilityInfoSignature AbilityInfoSignature;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FOnXPPercentChangedSignature OnXPPercentChangedSignature;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FOnPlayerStateChangedSignature OnPlayerLevelChangedDelegate;
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Widget Data")
	TObjectPtr<UDataTable>MessageWidgetDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo>AbilityInfo;

	void HealthChanged(const FOnAttributeChangeData& Data)const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data)const;
	void ManaChanged(const FOnAttributeChangeData& Data)const;
	void MaxManaChanged(const FOnAttributeChangeData& Data)const;

	void OnXPChanged(int32 InXP);
	void OnLevelChanged(int32 InLevel);

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);


	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC);
	static void ProcessAbilityStatic(const FGameplayAbilitySpec& AbilitySpec, UOverlayAuraWidgetController* Controller, UAuraAbilitySystemComponent* AuraASC);
};


template<typename T>
T* UOverlayAuraWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
