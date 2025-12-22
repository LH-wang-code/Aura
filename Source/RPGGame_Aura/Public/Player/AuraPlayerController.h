// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;
class ACharacter;
/**
 * 
 */
UCLASS()
class RPGGAME_AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;


	UFUNCTION(Client,Reliable)
	void ShowDamageText(float DamageText,ACharacter* TargetCharacter);
protected:
	virtual void BeginPlay()override;
	virtual void SetupInputComponent()override;

private:
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext>AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction>MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction>ShiftAction;


	void ShiftPressed(){bShiftKeyDawn=true;}
	void ShiftReleased(){bShiftKeyDawn=false;}
	bool bShiftKeyDawn;
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig>InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent>AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	FVector CachedDestination=FVector::ZeroVector;
	
	float FollowTime=0.f;
	float ShortPressThreshold=0.5f;
	bool bAutoRunning =false;
	bool bTargeting=false;
	
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius=50.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USplineComponent>Spline;

	void AutoRun();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageTextComponent>DamageTextComponentClass;
};
