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
void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//UKismetSystemLibrary::PrintString(this,FString("ActivateAbility(C++)"),true,true,FLinearColor::Yellow,3);


	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	
	const bool bIsServer=GetAvatarActorFromActorInfo()->HasAuthority();

	if (!bIsServer)return;

	ICombatInterface* CombatInterface=Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		

		const FVector SocketLocation= ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),FAuraGameplayTags::Get().CombatSocket_Weapon);

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

		for (auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
		}
		
		Projectile->DamageEffectSpecHandle = SpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}
}
