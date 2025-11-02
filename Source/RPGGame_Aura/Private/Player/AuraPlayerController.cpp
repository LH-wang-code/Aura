// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Ability/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "InterAction/EnemyInterface.h"
#include "Logging/LogMacros.h"
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline=CreateDefaultSubobject<USplineComponent>("Spline");
	
}
void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning)return;
	if (APawn* ControlledPawn=GetPawn())
	{
		const FVector LocationOnSpline=Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		const FVector Direction=Spline->FindDirectionClosestToWorldLocation(LocationOnSpline,ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);


		const float DistanceToDestination=(LocationOnSpline-CachedDestination).Length();
		if (DistanceToDestination<=AutoRunAcceptanceRadius)
		{
			bAutoRunning=false;
		}
		
	}
}
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::CursorTrace()
{
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

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent=Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
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
	//GEngine->AddOnScreenDebugMessage(1,3.f,FColor::Red,*InputTag.ToString());
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting=ThisActor?true:false;
		bAutoRunning=false;
	}
	

}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(2,3.f,FColor::Blue,*InputTag.ToString());
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}
	
	if (bTargeting)
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	}
	else
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime<=ShortPressThreshold&&ControlledPawn)
		{
			//UE_LOG(LogTemp,Warning,TEXT("Spline Points"));
			
			if (UNavigationPath* NavPath=UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlledPawn->GetActorLocation(),CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc:NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc,ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc,10.f,10.f,FColor::Green,false,5.f);
					UE_LOG(LogTemp,Warning,TEXT("Spline Points"));
				}
				CachedDestination=NavPath->PathPoints.Last();
				bAutoRunning=true;
				
			}
		}
		FollowTime=0.f;
		bTargeting=false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(3,3.f,FColor::Green,*InputTag.ToString());

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (bTargeting)
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		
	}
	else
	{
		FollowTime+=GetWorld()->GetDeltaSeconds();
		if (GetHitResultUnderCursor(ECC_Visibility, false, CursorHit))
		{
			CachedDestination=CursorHit.ImpactPoint;
		}


		if (APawn* ControllerPawn=GetPawn())
		{
			const FVector WorldDirection=CachedDestination - ControllerPawn->GetActorLocation();
			ControllerPawn->AddMovementInput(WorldDirection);
			
		}
	}
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
