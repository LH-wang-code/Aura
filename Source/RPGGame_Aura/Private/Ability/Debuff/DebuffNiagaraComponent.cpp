// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Debuff/DebuffNiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"


UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;

}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("CombatInterface valid: %s"),
		CombatInterface ? TEXT("YES") : TEXT("NO"));
	UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
		UE_LOG(LogTemp, Warning, TEXT("Bind ASC: %s"), *GetNameSafe(GetOwner()));
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC) {

			InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);

			});
	}
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UDebuffNiagaraComponent::OnOwnerDeath);
	}
}

void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	//UE_LOG(LogTemp, Log, TEXT("DebuffTagChanged´¥·¢£¡Tag:%s Count:%d"), *CallbackTag.ToString(), NewCount);
	const bool bOwnerValid= IsValid(GetOwner());
	const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && ICombatInterface::Execute_bIsDead(GetOwner());
	if (bOwnerValid && bOwnerAlive&&NewCount > 0)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
