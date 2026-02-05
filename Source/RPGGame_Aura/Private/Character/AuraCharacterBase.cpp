


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

AAuraCharacterBase::AAuraCharacterBase()
{
 
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);

	Weapon = CreateDefaultSubobject< USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}


UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;

}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

bool AAuraCharacterBase::bIsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvator_Implementation()
{
	return this;
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{

	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);


	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
}
void AAuraCharacterBase::Dissolve()
{
	if (DissolveMaterial)
	{
		UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterial, this);
		GetMesh()->SetMaterial(0, MaterialInstance);
		StartupTimeline(MaterialInstance);
	}

	if (WeaponDissolveMaterial)
	{
		UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterial, this);
		Weapon->SetMaterial(0, MaterialInstance);
		WeaponStartupTimeline(MaterialInstance);
	}
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactAnim;
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& GameplayTag)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	if (IsValid(Weapon) && GameplayTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (GameplayTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (GameplayTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	return FVector();
}

TArray<FTaggedMontageInfo> AAuraCharacterBase::GetAttackMontageTag_Implementation()
{
	return MontageInfo;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontageInfo AAuraCharacterBase::GetTaggedMontageInfoByTag_Implementation(const FGameplayTag& Tag)
{
	for (FTaggedMontageInfo TaggedMontageInfo : MontageInfo)
	{
		if (TaggedMontageInfo.MontageTag == Tag)
		{
			return TaggedMontageInfo;
		}
	}
	return FTaggedMontageInfo();
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{

}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect>GameplayEffectClass, float Level)const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass,Level, ContextHandle);

	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());

}

void AAuraCharacterBase::InitializeDefaultAttributes()const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	if (!AbilitySystemComponent)return;
	UAuraAbilitySystemComponent* AuraASC=CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	
	if(!HasAuthority())return;

	AuraASC->AddCharacterAbilities(StartupAbilities);
	
}



