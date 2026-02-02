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
class UWidgetComponent;
struct FInputActionValue;

UCLASS()
class CPP_ASSIGNMENT_8_JCM_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADrone();

	UFUNCTION(BlueprintPure, Category = "Drone|Health")
	int32 GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Drone|Health")
	void AddHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Drone|Movement")
	void SetSpeedMultiplier(float Multiplier);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Components")
	UWidgetComponent* OverHeadWidget;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent
		const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Drone|Health")
	virtual void OnDeath();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ApplyPoison(float Duration, float Damage);
	void PoisonTick();

	void ApplySlow(float Duration, float SlowMultiplier);
	void SlowTick();

	void UpdateOverHeadHP();


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

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone|Movement")
	float MoveSpeed = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone|Movement")
	float RotateSpeed = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone|Movement")
	float StableSpeed = 50.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Movement")
	float SpeedMultiplier = 1.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Animation")
	UAnimationAsset* LoopAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Health")
	float Health;


	FTimerHandle PoisonTimerHandle;
	int32 RemainingPoisonTicks;
	float CurrentPoisonDamage;

	FTimerHandle SlowTimerHandle;
	int32 RemainingSlowTicks;



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
