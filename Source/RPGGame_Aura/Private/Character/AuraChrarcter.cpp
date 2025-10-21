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
#include "Ability/AuraAbilitySystemComponent.h"
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
	InitAbilityActorInfo();
}


//客户端接收到 PlayerState 的复制更新时自动调用，同步数据
void AAuraChrarcter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void AAuraChrarcter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (!AuraPlayerState) return;

	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// 确保 Controller 存在
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		// 有时此时 HUD 还没创建出来
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
		else
		{
			// 延迟执行一次，HUD 创建通常在下一帧
			FTimerHandle DelayHandle;
			GetWorld()->GetTimerManager().SetTimer(DelayHandle, [AuraPlayerController, AuraPlayerState, this]()
				{
					if (AAuraHUD* AuraHUDLater = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
					{
						AuraHUDLater->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
						UE_LOG(LogTemp, Warning, TEXT("InitOverlay delayed executed successfully"));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("HUD still not valid after delay"));
					}
				}, 0.2f, false);
		}
	}
	//if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	//{
	//	if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
	//	{
	//		AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
	//	}
	//}


	InitializePrimaryAttributes();

}


