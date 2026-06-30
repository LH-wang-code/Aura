// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "../RPGGame_Aura.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Logging/LogMacros.h"
#include "Ability/AuraAbilitySystemFunctionLibrary.h"
AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	Sphere=CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed=550.f;
	ProjectileMovement->MaxSpeed=550.f;
	ProjectileMovement->ProjectileGravityScale=0.f;
	
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AAuraProjectile::OnSphereOverlap);
	AudioComponent=UGameplayStatics::SpawnSoundAttached(LoopSound,GetRootComponent());
}

void AAuraProjectile::Destroyed()
{

	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent->DestroyComponent();
	}
	if (!bHit&& !HasAuthority())
	{
		OnHit();

	}


	Super::Destroyed();
}

void AAuraProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent->DestroyComponent();
	}
	bHit = true;
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}
	AActor* SourceAvatarActor=DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor)return;
	//UE_LOG(LogTemp, Warning, TEXT("Projectile has Overlapped!!!"));
	if (!UAuraAbilitySystemFunctionLibrary::IsNotFriend(SourceAvatarActor, OtherActor))
	{
		return;
	}
	if (!bHit)
	{
		OnHit();
	}
	if (HasAuthority())
	{

		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{

			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;


			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if (bKnockback)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				const FVector KnockbackDirection = Rotation.Vector();

				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
				DamageEffectParams.KnockbackForce = KnockbackForce;
			}

			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemFunctionLibrary::ApplyGameplayEffect(DamageEffectParams);
		}
		Destroy();
	}
	else
	{
		bHit=true;
	}
}


