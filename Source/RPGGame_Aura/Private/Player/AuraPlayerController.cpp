// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
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
		LastActor is null and ThisActor is null   ʲôҲ����
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
	//// ȷ�����Ǳ��ؿ������ų��Ի�ȡ������ϵͳ
	//if (!IsLocalController())
	//{
	//	UE_LOG(LogTemp, Verbose, TEXT("Not a local controller, skipping EnhancedInput setup."));
	//	return;
	//}
	//ULocalPlayer* LocalPlayer = GetLocalPlayer();
	//if (!LocalPlayer)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("GetLocalPlayer returned nullptr. Delaying EnhancedInput setup."));
	//	// ��ѡ���ӳ�һ֡���Ի��� Possess ʱ����
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
	//UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	UAuraInputComponent* AuraInputComponent=CastChecked<UAuraInputComponent>(InputComponent);
	

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig,this,&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1,3.f,FColor::Red,*InputTag.ToString());
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2,3.f,FColor::Blue,*InputTag.ToString());
	
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3,3.f,FColor::Green,*InputTag.ToString());
	
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
