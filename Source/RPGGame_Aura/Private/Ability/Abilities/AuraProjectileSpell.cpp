// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Ability/AuraAttributeSet.h"
#include "AuraAbilityTypes.h"
#include "Logging/LogMacros.h"
#include <../../../../../../../Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h>

FString UAuraProjectileSpell::GetDescription(int32 Level)
{
	const int32 ScalaDamage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n<Defauta>launches a bolt of fine, exploding on impact and depling: </><Damage></><Damage>Fire Damage with a chance to burn</>\n\n<Small>Level:</><Level>%d</>"), ScalaDamage, Level);
	}
	else
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n<Defauta>launches %d bolt of fine, exploding on impact and depling: </><Damage></><Damage>Fire Damage with a chance to burn</>\n\n<Small>Level:</><Level>%d</>"), FMath::Min(Level, NumProjectiles), ScalaDamage, Level);

	}
}

FString UAuraProjectileSpell::GetNextLevelDescription(int32 Level)
{
	const int32 ScalaDamage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level + 1);

	return FString::Printf(TEXT("<Title>FIRE BOLT</>\n<Defauta>launches %d bolt of fine, exploding on impact and depling: </><Damage></><Damage>Fire Damage with a chance to burn</>\n\n<Small>Level:</><Level>%d</>"), FMath::Min(Level + 1, NumProjectiles), ScalaDamage, Level + 1);
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//UKismetSystemLibrary::PrintString(this,FString("ActivateAbility(C++)"),true,true,FLinearColor::Yellow,3);


	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation,const FGameplayTag& GameplayTag)
{
	
	const bool bIsServer=GetAvatarActorFromActorInfo()->HasAuthority();

	if (!bIsServer)return;

	ICombatInterface* CombatInterface=Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation= ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),GameplayTag);

		FRotator Rotation=(ProjectileTargetLocation-SocketLocation).Rotation();
		//Rotation.Pitch=0.f;
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		AAuraProjectile* Projectile=GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClass();

		Projectile->FinishSpawning(SpawnTransform);
	}
}
