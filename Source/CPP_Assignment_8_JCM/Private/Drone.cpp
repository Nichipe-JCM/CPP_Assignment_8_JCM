// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "PawnController.h"

#pragma region Construction
#pragma region Construction
// Sets default values
ADrone::ADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetSimulatePhysics(false);
	CapsuleComponent->SetEnableGravity(false);

	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(RootComponent);
	DroneMesh->SetSimulatePhysics(false);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);
	ArrowComponent->SetHiddenInGame(false);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DroneMeshAsset(TEXT("/Game/Fab/Free_Sci-Fi_Drone___Full_Pack_In_Description/SM_Drone.SM_Drone"));
	if (DroneMeshAsset.Succeeded()) {
		DroneMesh->SetSkeletalMesh(DroneMeshAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> LoopAnimationAsset(TEXT("/Game/Fab/Free_Sci-Fi_Drone___Full_Pack_In_Description/AS_Drone_Loop.AS_Drone_Loop"));
	if (LoopAnimationAsset.Succeeded()) {
		LoopAnimation = LoopAnimationAsset.Object;
	}
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();

	if(DroneMesh && LoopAnimation)
	{
		DroneMesh->PlayAnimation(LoopAnimation, true);
	}	
	
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (APawnController* PawnController = Cast<APawnController>(GetController())) {
			if (PawnController->DroneMoveAction) {
				EnhancedInput->BindAction(
					PawnController->DroneMoveAction,
					ETriggerEvent::Triggered,
					this,
					&ADrone::Move
				);
			}
			if (PawnController->DroneLookAction) {
				EnhancedInput->BindAction(
					PawnController->DroneLookAction,
					ETriggerEvent::Triggered,
					this,
					&ADrone::Look
				);
			}
			if (PawnController->DroneAscendAction) {
				EnhancedInput->BindAction(
					PawnController->DroneAscendAction,
					ETriggerEvent::Triggered,
					this,
					&ADrone::Ascend
				);
			}
			if (PawnController->DroneRollAction) {
				EnhancedInput->BindAction(
					PawnController->DroneRollAction,
					ETriggerEvent::Triggered,
					this,
					&ADrone::Roll
				);
			}
			if (PawnController->DroneStableAction) {
				EnhancedInput->BindAction(
					PawnController->DroneStableAction,
					ETriggerEvent::Triggered,
					this,
					&ADrone::Stable
				);
			}
		}
	}
}

void ADrone::Move(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (InputVector.IsZero()) return;

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FVector DeltaLocation(
		InputVector.X * MoveSpeed * DeltaTime,
		InputVector.Y * MoveSpeed * DeltaTime,
		0.0f);
	
	AddActorLocalOffset(DeltaLocation, true);
}

void ADrone::Look(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (InputVector.IsZero()) return;

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = InputVector.X * RotateSpeed * DeltaTime;
	DeltaRotation.Pitch = InputVector.Y * RotateSpeed * DeltaTime;

	AddActorLocalRotation(DeltaRotation);
}

void ADrone::Ascend(const FInputActionValue& Value)
{
	float InputFloat = Value.Get<float>();
	if (InputFloat == 0.0f) return;

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FVector DeltaLocation(
		0.0f,
		0.0f,
		InputFloat * MoveSpeed * DeltaTime);
	
	AddActorLocalOffset(DeltaLocation, true);
}

void ADrone::Roll(const FInputActionValue& Value)
{
	float InputFloat = Value.Get<float>();
	if (InputFloat == 0.0f) return;

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Roll = InputFloat * RotateSpeed * DeltaTime;

	AddActorLocalRotation(DeltaRotation);
}

void ADrone::Stable(const FInputActionValue& Value)
{
	if (!Value.Get<bool>()) return;

	float DeltaTime = GetWorld()->GetDeltaSeconds();
		
	FQuat CurrentQuat = GetActorRotation().Quaternion();
	FRotator TargetRotator = FRotator(0.0f, GetActorRotation().Yaw, 0.0f);
	FQuat TargetQuat = TargetRotator.Quaternion();

	float RemainingAngle = CurrentQuat.AngularDistance(TargetQuat);

	if (RemainingAngle <= KINDA_SMALL_NUMBER) return;

	float DeltaAngle = FMath::DegreesToRadians(StableSpeed) * DeltaTime;
	float Alpha = FMath::Clamp(DeltaAngle / RemainingAngle, 0.0f, 1.0f);

	FQuat NewQuat = FQuat::Slerp(CurrentQuat, TargetQuat, Alpha);

	SetActorRotation(NewQuat);
}
