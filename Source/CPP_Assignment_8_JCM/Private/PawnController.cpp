// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

APawnController::APawnController() :
	PawnInputMappingContext(nullptr),
	DroneInputMappingContext(nullptr),
	GlobalInputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	DroneMoveAction(nullptr),
	DroneLookAction(nullptr),
	DroneAscendAction(nullptr),
	DroneRollAction(nullptr),
	DroneStableAction(nullptr),
	SwitchToPawnAction(nullptr),
	SwitchToDroneAction(nullptr),
	CharacterPawn(nullptr),
	DronePawn(nullptr)
{
}

void APawnController::BeginPlay()
{
	Super::BeginPlay();

	CharacterPawn = GetPawn();

	if (DronePawnClass)
	{
		AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), DronePawnClass);
		DronePawn = Cast<APawn>(FoundActor);

		if (!DronePawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("DronePawnClass found, but no drone pawn instance found."));
		}
	}

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (GlobalInputMappingContext)
			{
				Subsystem->AddMappingContext(GlobalInputMappingContext, 0);
			}

			if (PawnInputMappingContext)
			{
				Subsystem->AddMappingContext(PawnInputMappingContext, 1);
			}
		}
	}
}

void APawnController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (SwitchToPawnAction)
		{
			EnhancedInput->BindAction(
				SwitchToPawnAction,
				ETriggerEvent::Triggered,
				this,
				&APawnController::SwitchToPawn
			);
		}
		if (SwitchToDroneAction)
		{
			EnhancedInput->BindAction(
				SwitchToDroneAction,
				ETriggerEvent::Triggered,
				this,
				&APawnController::SwitchToDrone
			);
		}
	}
}

void APawnController::SwitchToPawn(const FInputActionValue& Value)
{
	if (GetPawn() == CharacterPawn || !CharacterPawn) return;
	UnPossess();
	Possess(CharacterPawn);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (DroneInputMappingContext)
		{
			if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					Subsystem->RemoveMappingContext(DroneInputMappingContext);
				}
			}
		}
		if (PawnInputMappingContext)
		{
			if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					Subsystem->AddMappingContext(PawnInputMappingContext, 1);
				}
			}
		}
	}
}

void APawnController::SwitchToDrone(const FInputActionValue& Value)
{
	if (GetPawn() == DronePawn || !DronePawn) return;
	UnPossess();
	Possess(DronePawn);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (PawnInputMappingContext)
		{
			if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					Subsystem->RemoveMappingContext(PawnInputMappingContext);
				}
			}
		}
		if (DroneInputMappingContext)
		{
			if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					Subsystem->AddMappingContext(DroneInputMappingContext, 1);
				}
			}
		}
	}
}