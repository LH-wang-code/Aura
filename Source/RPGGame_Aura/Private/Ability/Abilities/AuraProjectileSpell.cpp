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

		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>>Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);
		
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
		
		const FAuraGameplayEffectContext* AuraContext =
			static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

		check(AuraContext);

		UE_LOG(LogTemp, Warning, TEXT("Critical=%d Blocked=%d"),
			AuraContext->IsCriticalHit(),
			AuraContext->IsBlockedHit());


		//meta Data
		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, ScaledDamage);
		Projectile->DamageEffectSpecHandle = SpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}
}
