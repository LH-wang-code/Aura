// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InterAction/EnemyInterface.h"
#include "Logging/LogMacros.h"
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	/*
		LastActor is null and ThisActor is null   什么也不做
		LastActor is null and ThisActor is valid  Highlight ThisActor
		LastActor is valid and ThisActor is null  UnHighlight LastActor
		LastActor is valid and ThisActor is valid Highlight ThisActor
		LastActor=ThisActor                       Do Nothing
	*/

	if (LastActor != ThisActor)
	{
		if (LastActor != nullptr)
		{
			LastActor->UnHighlightActor();

		}
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
	}


}

void AAuraPlayerController::BeginPlay()
{
	//Super::BeginPlay();

	////check(AuraContext);
	//// 确保这是本地控制器才尝试获取本地子系统
	//if (!IsLocalController())
	//{
	//	UE_LOG(LogTemp, Verbose, TEXT("Not a local controller, skipping EnhancedInput setup."));
	//	return;
	//}
	//ULocalPlayer* LocalPlayer = GetLocalPlayer();
	//if (!LocalPlayer)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("GetLocalPlayer returned nullptr. Delaying EnhancedInput setup."));
	//	// 可选择延迟一帧再试或在 Possess 时重试
	//	return;
	//}

	//UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//if (!SubSystem)
	//{
	//	return;
	//}
	//SubSystem->AddMappingContext(AuraContext, 0);

	//bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Default;

	//FInputModeGameAndUI InputModeData;
	//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//InputModeData.SetHideCursorDuringCapture(false);
	//SetInputMode(InputModeData);
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);


	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);

	}

}
