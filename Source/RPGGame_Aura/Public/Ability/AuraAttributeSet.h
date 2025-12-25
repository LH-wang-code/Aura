// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

 #define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);



USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC=nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor=nullptr;
	UPROPERTY()

	AController* SourceController=nullptr;
	UPROPERTY()

	ACharacter* SourceCharacter=nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()

	AController* TargetController = nullptr;
	UPROPERTY()

	ACharacter* TargetCharacter = nullptr;

};

//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr AttributeFuncPointer;

template<class T>
using TStaticFuncPtr= typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class RPGGAME_AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)override;

	//函数指针
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>>TagsToAttributes;



	/*
	*Primary Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "AttributeSets")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "AttributeSets")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "AttributeSets")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "AttributeSets")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

	/*
	*Second Attributes
		
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "AttributeSets")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "AttributeSets")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "AttributeSets")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "AttributeSets")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "AttributeSets")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "AttributeSets")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "AttributeSets")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "AttributeSets")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "AttributeSets")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "AttributeSets")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);


	///*
	//	*Secondary Attributes_Damage
	//*/
	//UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage_Fire, Category = "AttributeSets")
	//FGameplayAttributeData Damage_Fire;
	//ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Damage_Fire);

	//UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage_Lightning, Category = "AttributeSets")
	//FGameplayAttributeData Damage_Lightning;
	//ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Damage_Lightning);

	//UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage_Arcane, Category = "AttributeSets")
	//FGameplayAttributeData Damage_Arcane;
	//ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Damage_Arcane);

	//UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage_Physical, Category = "AttributeSets")
	//FGameplayAttributeData Damage_Physical;
	//ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Damage_Physical);


	/*
		*Secondary Attributes_ResistanceToDamage
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Resistance AttributeSets")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = "Resistance AttributeSets")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category = "Resistance AttributeSets")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance AttributeSets")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance);



	/*
	*	Vital Attributes
	*/

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Health,Category="AttributeSets")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "AttributeSets")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);


	/*
		Meta Attributes
	*/

	UPROPERTY(BlueprintReadOnly, Category = "MetaAttributeSets")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);



	UFUNCTION()
	void OnRep_Health(FGameplayAttributeData& OldHealth)const ;

	UFUNCTION()
	void OnRep_MaxHealth(FGameplayAttributeData& OldMaxHealth)const;
	
	UFUNCTION()
	void OnRep_Mana(FGameplayAttributeData& OldMana)const;

	UFUNCTION()
	void OnRep_MaxMana(FGameplayAttributeData& OldMaxMana)const;

	UFUNCTION()
	void OnRep_Strength(FGameplayAttributeData& OldStrength)const;

	UFUNCTION()
	void OnRep_Intelligence(FGameplayAttributeData& OldIntelligence)const;

	UFUNCTION()
	void OnRep_Vigor(FGameplayAttributeData& OldVigor)const;
	UFUNCTION()
	void OnRep_Resilience(FGameplayAttributeData& OldResilience)const;

	UFUNCTION()
	void OnRep_Armor(FGameplayAttributeData& OldArmor)const;


	UFUNCTION()
	void OnRep_ArmorPenetration(FGameplayAttributeData& OldArmorPenetration)const;

	UFUNCTION()
	void OnRep_BlockChance(FGameplayAttributeData& OldBlockChance)const;

	UFUNCTION()
	void OnRep_CriticalHitChance(FGameplayAttributeData& OldCriticalHitChance)const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(FGameplayAttributeData& OldCriticalHitDamage)const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(FGameplayAttributeData& OldCriticalHitResistance)const;

	UFUNCTION()
	void OnRep_HealthRegeneration(FGameplayAttributeData& OldHealthRegeneration)const;

	UFUNCTION()
	void OnRep_ManaRegeneration(FGameplayAttributeData& OldManaRegeneration)const;

	//UFUNCTION()
	//void OnRep_Damage_Fire(FGameplayAttributeData& OldDamage_Fire)const;

	//UFUNCTION()
	//void OnRep_Damage_Lightning(FGameplayAttributeData& OldDamage_Lightning)const;

	//UFUNCTION()
	//void OnRep_Damage_Arcane(FGameplayAttributeData& OldDamage_Arcane)const;

	//UFUNCTION()
	//void OnRep_Damage_Physical(FGameplayAttributeData& OldDamage_Physical)const;

	UFUNCTION()
	void OnRep_FireResistance(FGameplayAttributeData& OldFireResistance)const;

	UFUNCTION()
	void OnRep_LightningResistance(FGameplayAttributeData& OldLightningResistance)const;

	UFUNCTION()
	void OnRep_ArcaneResistance(FGameplayAttributeData& OldArcaneResistance)const;

	UFUNCTION()
	void OnRep_PhysicalResistance(FGameplayAttributeData& OldPhysicalResistance)const;




private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& Props)const;

};
