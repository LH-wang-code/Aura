// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	UE_LOG(LogTemp, Warning, TEXT(" FAuraGameplayTags::InitializeNativeGameplayTags called"));

	// Primary Attributes
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"), FString("Increases physical power and melee damage"));
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"), FString("Boosts stamina and overall survivability"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"), FString("Improves resistance to crowd control and debuffs"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"), FString("Enhances magical damage and mana efficiency"));

	// Secondary Attributes
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"), FString("Reduces incoming physical damage"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"), FString("Ignores a portion of enemy armor"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"), FString("Chance to block incoming attacks"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"), FString("Chance to deal critical damage"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"), FString("Bonus damage dealt on critical hits"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces damage from enemy critical hits"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"), FString("Restores health over time"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"), FString("Restores mana over time"));
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"), FString("Increases total health capacity"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"), FString("Increases total mana capacity"));


	//InputTags
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"), FString("Input Tag For Left Mouse Button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"), FString("Input Tag For Right Mouse Button"));  // 改为 RMB
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"), FString("Input Tag For Key 1"));  // 改为 1
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"), FString("Input Tag For Key 2"));  // 改为 2
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"), FString("Input Tag For Key 3"));  // 改为 3
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"), FString("Input Tag For Key 4"));  // 改为 4
	

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"), FString("Primary Damage"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"), FString("Fire Damage"));
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"), FString("Fire Damage"));
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"), FString("Arcane Damage"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"), FString("Physical Damage"));
	/*
	Resistance To Damage
	*/
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"), FString("Resistance to Fire"));
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"), FString("Resistance to Lightning"));
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"), FString("Resistance to Arcane"));
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"), FString("Resistance to Physical"));

	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Fire,GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);




	//Attack
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"), FString("Attack Abilities Tag"));
	
	//Effect
	GameplayTags.Effects_HitReact= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"), FString("Effects to Make goblin react"));  



	//Combat Sockets
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"), FString("CombatSocket Weapon"));
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"), FString("CombatSocket Hand"));
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"), FString("CombatSocket Hand"));

	//Montage Attack
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"), FString("Attack 1"));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"), FString("Attack 2"));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"), FString("Attack 3"));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"), FString("Attack 4"));
}
