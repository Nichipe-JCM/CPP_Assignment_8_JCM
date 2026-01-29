// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnClassAnimInstance.h"
#include "PawnClass.h"

void UPawnClassAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (APawn* Pawn = TryGetPawnOwner())
	{
		OwnerPawn = Cast<APawnClass>(Pawn);
		if(OwnerPawn)
		{
			PreviousLocation = OwnerPawn->GetActorLocation();
		}
	}
}

void UPawnClassAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwnerPawn || !OwnerPawn->IsValidLowLevel())
	{
		if (APawn* Pawn = TryGetPawnOwner())
		{
			OwnerPawn = Cast<APawnClass>(Pawn);
		}
		return;
	}

	if (DeltaSeconds <= 0.0f) return;


	FVector CurrentLocation = OwnerPawn->GetActorLocation();
	Velocity = (CurrentLocation - PreviousLocation) / DeltaSeconds;

	PreviousLocation = CurrentLocation;

	GroundSpeed = Velocity.Size2D();

	bShouldMove = GroundSpeed > 3.0f;

	bIsFalling = !OwnerPawn->GetIsGrounded();
}