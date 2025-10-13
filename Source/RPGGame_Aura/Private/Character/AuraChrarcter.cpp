// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraChrarcter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Ability/AuraAttributeSet.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"
AAuraChrarcter::AAuraChrarcter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

}

//控制器控制角色时调用，abillity给到角色
void AAuraChrarcter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilitySystemComponent();
}


//客户端接收到 PlayerState 的复制更新时自动调用，同步数据
void AAuraChrarcter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilitySystemComponent();
}

void AAuraChrarcter::InitAbilitySystemComponent()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	//UE_LOG(LogTemp, Warning, TEXT("ASC Init: %s"), *GetNameSafe(AbilitySystemComponent));


	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AAuraHUD* AuraHUD=Cast<AAuraHUD>(AuraPlayerController->GetHUD());
		AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);

	}

}