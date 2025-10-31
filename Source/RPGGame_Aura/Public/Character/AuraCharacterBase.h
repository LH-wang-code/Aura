// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "UObject/ObjectPtr.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"



class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
UCLASS(Abstract)
class RPGGAME_AURA_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:

	AAuraCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;
	UAttributeSet* GetAttributeSet()const { return AttributeSet; }

protected:
	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")

	TSubclassOf<UGameplayEffect>DefaultVitalAttributes;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")

	TSubclassOf<UGameplayEffect>DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")

	TSubclassOf<UGameplayEffect>DefaultSecondaryAttributes;
protected:

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();


	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect>GameplayEffectClass, float Level)const;


	void InitializeDefaultAttributes()const;

	void AddCharacterAbilities();
private:
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>>StartupAbilities;

};
