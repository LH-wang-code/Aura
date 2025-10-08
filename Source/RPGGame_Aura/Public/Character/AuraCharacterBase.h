// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

UCLASS(Abstract)
class RPGGAME_AURA_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	AAuraCharacterBase();


protected:
	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

protected:

	virtual void BeginPlay() override;



};
