// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UArrowComponent;
struct FInputActionValue;

UCLASS()
class CPP_ASSIGNMENT_8_JCM_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Components")
	UCapsuleComponent* CapsuleComponent; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Components")
	USkeletalMeshComponent* DroneMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Components")
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Components")
	UCameraComponent* Camera;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone|Movement")
	float MoveSpeed = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone|Movement")
	float RotateSpeed = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone|Movement")
	float StableSpeed = 50.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Animation")
	UAnimationAsset* LoopAnimation;

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Ascend(const FInputActionValue& Value);
	UFUNCTION()
	void Roll(const FInputActionValue& Value);
	UFUNCTION()
	void Stable(const FInputActionValue& Value);

};
