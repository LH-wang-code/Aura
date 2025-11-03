// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj=NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled=IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
		
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle =GetAbilitySpecHandle();
		const FPredictionKey AcitvationPreDictionKey=GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(),GetActivationPredictionKey()).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate=AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,AcitvationPreDictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}

}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	APlayerController* PC=Ability->GetCurrentActorInfo()->PlayerController.Get();

	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data=new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult=CursorHit;
	DataHandle.Add(Data);
	//没用到。但是要传递	
	FGameplayTag ApplicationTag;
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,ApplicationTag,
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
}
