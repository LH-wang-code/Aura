// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindNearestPlayer.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class RPGGAME_AURA_API UBTService_FindNearestPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;


	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Selector")
	FBlackboardKeySelector TargetToFollow;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Selector")
	FBlackboardKeySelector DistanceToTarget;

	

};
