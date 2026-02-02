// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Components/WidgetComponent.h"
#include "UI/WidgetController/OverlayAuraWidgetController.h"
#include "Ability/Data/CharacterClassInfo.h" 
#include "AuraEnemy.generated.h"


class UBehaviorTree;
class AAuraAIController;
/**
 * 
 */

UCLASS()
class RPGGAME_AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()
	
public:


	virtual void PossessedBy(AController* NewController)override;

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

	void HitReactTagChanged(const FGameplayTag CallbackTag,int32 NewCount);

	virtual void Die()override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

protected:

	virtual void BeginPlay()override;
	virtual void InitAbilityActorInfo()override;
	virtual void InitializeDefaultAttributes()const override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat")
	float WalkSpeedBase = 250.f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Combat")
	float LifeSpan = 5.f;
	UPROPERTY(BlueprintReadOnly,Category="Combat")
	bool bHitReact = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor>TargetCombat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass=ECharacterClass::Warrior;


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> EnemyHealthWidget;


	//AIController
	UPROPERTY(EditAnywhere,Category="AI")
	TObjectPtr<UBehaviorTree>BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<AAuraAIController>AuraAIController;
};
