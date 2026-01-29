// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PawnClassAnimInstance.generated.h"

class APawnClass;

UCLASS()
class CPP_ASSIGNMENT_8_JCM_API UPawnClassAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnClassAnimInstance|References")
	APawnClass* OwnerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnClassAnimInstance|Data")
	FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnClassAnimInstance|Data")
	float GroundSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnClassAnimInstance|Data")
	bool bShouldMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnClassAnimInstance|Data")
	bool bIsFalling;

	FVector PreviousLocation;
};
