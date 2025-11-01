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

protected:
	virtual void BeginPlay()override;
	virtual void SetupInputComponent()override;

private:
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext>AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction>MoveAction;
	
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig>InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

};
