// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Components/WidgetComponent.h"
#include "UI/WidgetController/OverlayAuraWidgetController.h"
#include "Ability/Data/CharacterClassInfo.h" 
#include "AuraEnemy.generated.h"

/**
 * 
 */

UCLASS()
class RPGGAME_AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()
	
public:

	//IEnemyInterface
	AAuraEnemy();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	//Combat Interface

	virtual int32 GetPlayerLevel() override;

	UPROPERTY(VisibleAnywhere,BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;


protected:
	virtual void BeginPlay()override;
	virtual void InitAbilityActorInfo()override;
	virtual void InitializeDefaultAttributes()const override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass=ECharacterClass::Marrier;


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> EnemyHealthWidget;

};
