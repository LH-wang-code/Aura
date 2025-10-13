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

//���������ƽ�ɫʱ���ã�abillity������ɫ
void AAuraChrarcter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilitySystemComponent();
}


//�ͻ��˽��յ� PlayerState �ĸ��Ƹ���ʱ�Զ����ã�ͬ������
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