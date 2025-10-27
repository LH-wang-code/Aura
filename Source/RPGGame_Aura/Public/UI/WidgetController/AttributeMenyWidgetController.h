// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Ability/Data/AttributeInfo.h"
#include "Delegates/DelegateCombinations.h"
#include "AttributeMenyWidgetController.generated.h"

class UAttributeInfo;
struct FAuraAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class RPGGAME_AURA_API UAttributeMenyWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()


public:
	virtual void BroadcastInitialValues()override;
	virtual void BindCallbacksToDependencies()override;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;


protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UAttributeInfo>AttributeInfo;

};
