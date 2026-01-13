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
class UAnimMontage;
class UMaterialInstance;
class UMaterialInstanceDynamic;
UCLASS(Abstract)
class RPGGAME_AURA_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:

	AAuraCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;
	UAttributeSet* GetAttributeSet()const { return AttributeSet; }


	virtual UAnimMontage* GetHitReactMontage_Implementation()override;
	virtual void Die() override;

	virtual bool bIsDead_Implementation()const override;
	virtual AActor* GetAvator_Implementation() override;


	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();



	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartupTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponStartupTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
protected:
	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere,Category="Combat")
	FName WeaponTipSocketName;

	virtual FVector GetCombatSocketLocation_Implementation() override;
	
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

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance>DissolveMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance>WeaponDissolveMaterial;


	bool bDead = false;

protected:

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();


	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect>GameplayEffectClass, float Level)const;


	virtual void InitializeDefaultAttributes()const;

	void AddCharacterAbilities();
private:
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>>StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage>HitReactAnim;

};
