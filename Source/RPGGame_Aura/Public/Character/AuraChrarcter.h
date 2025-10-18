// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraChrarcter.generated.h"




/**
 * 
 */
UCLASS()
class RPGGAME_AURA_API AAuraChrarcter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:

	AAuraChrarcter();


protected:
	virtual void PossessedBy(AController* NewController)override;
	virtual void OnRep_PlayerState()override;
	//virtual void Tick(float DeltaTime)override;
private:
	

	virtual void InitAbilityActorInfo()override;


	
};
