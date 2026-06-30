// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Abilities/AuraBeamSpell.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "Ability/AuraAbilitySystemFunctionLibrary.h"
void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	}
}

void UAuraBeamSpell::StoreOwnerVaribles()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*>ActorsToIgnore;
			FHitResult HitResult;
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation, 
				BeamTargetLocation,
				10.f, 
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::ForDuration,
				HitResult, 
				true
			);
			if (HitResult.bBlockingHit)
			{
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*>ActorsToIgnore;

	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	TArray<AActor*>OverlappingActors;

	UAuraAbilitySystemFunctionLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		850.f,
		MouseHitActor->GetActorLocation()
		);

	//int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel(), MaxNumShockTargets);
	int32 numAdditionTargets = 5;

	UAuraAbilitySystemFunctionLibrary::GetClosestTargets(numAdditionTargets, OverlappingActors, OutAdditionalTargets, MouseHitActor->GetActorLocation());


}
