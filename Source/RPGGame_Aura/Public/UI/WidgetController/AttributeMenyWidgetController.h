// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenyWidgetController.generated.h"



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
	
	
};
