// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnClass.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class CPP_ASSIGNMENT_8_JCM_API APawnClass : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnClass|Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnClass|Components")
	USkeletalMeshComponent* PawnMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnClass|Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnClass|Components")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnClass|Movement")
	float MoveSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnClass|Movement")
	float LookSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnClass|Movement")
	float GravityStrength = 980.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnClass|Movement")
	float JumpStrength = 420.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnClass|Debug")
	float VerticalVelocity = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnClass|Debug")	
	bool isGrounded = false;

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "PawnClass|Getter")
	bool GetIsGrounded() const { return isGrounded; }


};
