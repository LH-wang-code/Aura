// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Abilities/AuraFireBolts.h"
#include <../../../../../../../Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h>
#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "Ability/AuraAbilitySystemFunctionLibrary.h"
#include "Actor/AuraProjectile.h"
FString UAuraFireBolts::GetDescription(int32 Level)
{
	const int32 ScalaDamage = Damage.GetValueAtLevel(Level);

	const float ManaCost = GetManaCost(Level);
	const float cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n"
			"<Small>Level:</><Level>%d</>"
			"<Small>ManaCost:</><ManaCost>%.1f</>"
			"<Small>Cooldown:</><CoolDown>%.1f</>"

			"<Defauta>launches a bolt of fine, exploding on impact and depling: </>"
			"<Damage>< / ><Damage>Fire Damage with a chance to burn< / >"
		""), Level, ManaCost ,cooldown, ScalaDamage);
	}
	else
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n<Defauta>launches %d bolt of fine, exploding on impact and depling: </><Damage></><Damage>Fire Damage with a chance to burn</>\n\n<Small>Level:</><Level>%d</>"), FMath::Min(Level, NumProjectiles), ScalaDamage, Level);

	}
}

FString UAuraFireBolts::GetNextLevelDescription(int32 Level)
{
	const int32 ScalaDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level+1);
	const float cooldown = GetCooldown(Level+1);
	return FString::Printf(TEXT("<Title>FIRE BOLT</>\n"
		"<Small>Level:</><Level>%d</>"
		"<Small>ManaCost:</><ManaCost>%.1f</>"
		"<Small>Cooldown:</><CoolDown>%.1f</>"

		"<Defauta>launches a bolt of fine, exploding on impact and depling: </>"
		"<Damage>< / ><Damage>Fire Damage with a chance to burn< / >"
		""), Level+1, ManaCost, cooldown, ScalaDamage);
}

void UAuraFireBolts::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch,float PitchOverride, AActor* HomingActor)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)return;
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag
	);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}


	const FVector Forward = Rotation.Vector();

	TArray<FVector>Directions = UAuraAbilitySystemFunctionLibrary::EvenlyRotatedVectors(Forward, FVector::UpVector, ProjectileSpread, MaxNumProjectiles);
	TArray<FRotator>Rotations = UAuraAbilitySystemFunctionLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, MaxNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClass();

		Projectile->FinishSpawning(SpawnTransform);
	}
}
