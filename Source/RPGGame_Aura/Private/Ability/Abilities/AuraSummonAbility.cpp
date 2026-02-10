// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Abilities/AuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetActorSpawnLocation()
{
	TArray<FVector>MinionLocation;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();

	float AveRotation = MinSpread / MinionCount;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-MinSpread / 2.f, FVector::UpVector);

	for (int i = 0; i < MinionCount; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(AveRotation * i, FVector::UpVector);
		FVector RandLocation = Location + Direction * FMath::RandRange(MinDistance, MaxDistance);
		
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, RandLocation + FVector(0, 0, 500.f), RandLocation - FVector(0, 0, 500.f), ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			RandLocation = Hit.ImpactPoint;
		}
		MinionLocation.Add(RandLocation);
		//DrawDebugSphere(GetWorld(), RandLocation, 15.f,12, FColor::Green, false,3.f);

	}
	//const FVector LeftOfSpread = Forward.RotateAngleAxis(MinSpread / 2.f, FVector::UpVector);
	//UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxDistance,4.f,FLinearColor::Blue,3.f);
	//DrawDebugSphere(GetWorld(), Location + MinDistance * LeftOfSpread, 15.f,12, FColor::Green, false,3.f);
	//DrawDebugSphere(GetWorld(), Location + MaxDistance * LeftOfSpread, 15.f, 12, FColor::Green, false, 3.f);
	//const FVector RightOfSpread = Forward.RotateAngleAxis(-MinSpread / 2.f, FVector::UpVector);
	//UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + RightOfSpread * MaxDistance, 4.f, FLinearColor::Gray,3.f);
	//DrawDebugSphere(GetWorld(), Location + MinDistance * RightOfSpread, 15.f, 12, FColor::Green, false, 3.f);
	//DrawDebugSphere(GetWorld(), Location + MaxDistance * RightOfSpread, 15.f, 12, FColor::Green, false, 3.f);
	return MinionLocation;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	int RandomIndex = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[RandomIndex];
}
