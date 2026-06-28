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
#include "NiagaraComponent.h"
AAuraChrarcter::AAuraChrarcter()
{


	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent,USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}

//控制器控制角色时调用，abillity给到角色
void AAuraChrarcter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	AddCharacterAbilities();
}


//客户端接收到 PlayerState 的复制更新时自动调用，同步数据
void AAuraChrarcter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
	AddCharacterAbilities();
}


int32 AAuraChrarcter::GetSpellPoints_Implementation()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

int32 AAuraChrarcter::GetAttributePoints_Implementation()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraChrarcter::FindLevelForXP_Implementation(int32 XP)const 
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(XP);
}

int32 AAuraChrarcter::GetAttributePointsReward_Implementation(int32 Level) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInfo[Level].AttributePointAward;
}

int32 AAuraChrarcter::GetSpellPointsReward_Implementation(int32 Level) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInfo[Level].SpellPointAward;
}

void AAuraChrarcter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	//TODO
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState-> AddToAttributePoints(InAttributePoints);

}

void AAuraChrarcter::AddToSpellPoints_Implementation(int32 InSPellPoints)
{
	//TODO
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InSPellPoints);
}

void AAuraChrarcter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InPlayerLevel);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());

	}
}

int32 AAuraChrarcter::GetXP_Implementation()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerXP();
}

int32 AAuraChrarcter::GetPlayerLevel_Implementation()
{
	
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraChrarcter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}
void AAuraChrarcter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = CameraComponent->GetComponentLocation();

		const FVector LevelUpNiagaraComponentLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator TOCameraRotation = (CameraLocation - LevelUpNiagaraComponentLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(TOCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}

}

void AAuraChrarcter::AddToXP_Implementation(int32 XP)
{
	if (AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>())
	{
		AuraPlayerState->AddToXP(XP);
		UE_LOG(LogTemp, Warning, TEXT("PlayerState is not exist"));

	}
	UE_LOG(LogTemp, Warning,TEXT("AddToXP_Implementation Executed"));

}

void AAuraChrarcter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (!AuraPlayerState) return;

	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	AAuraPlayerController* PC = Cast<AAuraPlayerController>(GetController());
	if (!PC) return;
	auto InitHUD = [this, PC, AuraPlayerState]()
		{
			if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
			{
				AuraHUD->InitOverlay(PC, AuraPlayerState, AbilitySystemComponent, AttributeSet);
				UE_LOG(LogTemp, Warning, TEXT("InitOverlay success"));
			}
		};


	if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
	{
		InitHUD();
	}
	else
	{
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(
			Handle,
			InitHUD,
			0.0f,   
			false
		);
	}
	// 确保 Controller 存在
	//if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	//{
	//	// 有时此时 HUD 还没创建出来
	//	if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
	//	{
	//		AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
	//	}
	//	else
	//	{
	//		// 延迟执行一次，HUD 创建通常在下一帧
	//		FTimerHandle DelayHandle;
	//		GetWorld()->GetTimerManager().SetTimer(DelayHandle, [AuraPlayerController, AuraPlayerState, this]()
	//			{
	//				if (AAuraHUD* AuraHUDLater = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
	//				{
	//					AuraHUDLater->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
	//					UE_LOG(LogTemp, Warning, TEXT("InitOverlay delayed executed successfully"));
	//				}
	//				else
	//				{
	//					UE_LOG(LogTemp, Warning, TEXT("HUD still not valid after delay"));
	//				}
	//			}, 0.2f, false);
	//	}
	//}
	//if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	//{
	//	if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
	//	{
	//		AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
	//	}
	//}


	InitializeDefaultAttributes();

}



