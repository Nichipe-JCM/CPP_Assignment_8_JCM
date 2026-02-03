// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpartaGameState.h"
#include "Components/TextBlock.h"
#include "SpartaGameInstance.h"
#include "Components/Button.h"

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
	DronePawn(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),	
	MainMenuWidgetInstance(nullptr)
{
}

void APawnController::BeginPlay()
{
	Super::BeginPlay();

	CharacterPawn = GetPawn();
	FString CurrentMapName = GetWorld()->GetMapName();

	

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
	if (CurrentMapName.Contains("Advanced"))
	{
		if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (DroneInputMappingContext)
				{
					Subsystem->RemoveMappingContext(PawnInputMappingContext);
					Subsystem->AddMappingContext(DroneInputMappingContext, 1);
				}
			}
		}
	}
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* APawnController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void APawnController::ShowMainMenu(bool bIsRestart)
{
	bIsGameOverMenu = bIsRestart;
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Start")));	
			}
		}

		if (UTextBlock* MainScreenText1 = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("MainScreenText1"))))
		{
			MainScreenText1->SetVisibility(bIsRestart ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		}
		if (UTextBlock* MainScreenText2 = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("MainScreenText2"))))
		{
			MainScreenText2->SetVisibility(bIsRestart ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		}
		UFunction* PlayMainAnimFunc = MainMenuWidgetInstance->FindFunction(TEXT("PlayMainAnim"));
		if (PlayMainAnimFunc)
		{
			MainMenuWidgetInstance->ProcessEvent(PlayMainAnimFunc, nullptr);
		}

		if (UTextBlock* QuitButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("QuitButtonText"))))
		{
			QuitButtonText->SetText(FText::FromString(bIsRestart ? TEXT("Main Menu") : TEXT("Quit")));
		}
		if (UButton* QuitButton = Cast<UButton>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("QuitButton"))))
		{
			QuitButton->OnClicked.RemoveAll(this);
			QuitButton->OnClicked.AddDynamic(this, &APawnController::OnQuitButtonClicked);
		}

		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(TEXT("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TotalScoreText"))))
			{
				if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
					FString::Printf(TEXT("Total Score: %d"), SpartaGameInstance->TotalScore)));
				}
			}
		}
	}
}

void APawnController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
		
		ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
		if (SpartaGameState)
		{
			SpartaGameState->UpdateHUD();
		}
	}
}

void APawnController::StartGame()
{
	if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SpartaGameInstance->CurrentLevelIndex = 0;
		SpartaGameInstance->TotalScore = 0;
	}
	UGameplayStatics::OpenLevel(
		GetWorld(),
		FName("BasicLevel")
	);
	SetPause(false);
}

void APawnController::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, true);
}

void APawnController::OnQuitButtonClicked()
{
	if (bIsGameOverMenu)
	{
		SetPause(false);
		UGameplayStatics::OpenLevel(GetWorld(), FName("MenuLevel"));
		return;
	}

	QuitGame();
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

void APawnController::PoisonHUD(int32 RemainingTime, bool bIsVisible)
{
	if (!HUDWidgetInstance) return;
	UTextBlock* PoisonTimer = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("PoisonTimer")));
	if (!PoisonTimer) return;
	if (bIsVisible)
	{
		PoisonTimer->SetText(FText::FromString(
			FString::Printf(TEXT("Poison: %d"), RemainingTime)
		));
		PoisonTimer->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PoisonTimer->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APawnController::SlowHUD(int32 RemainingTime, bool bIsVisible)
{
	if (!HUDWidgetInstance) return;
	UTextBlock* SlowTimer = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("SlowTimer")));
	if (!SlowTimer) return;
	if (bIsVisible)
	{
		SlowTimer->SetText(FText::FromString(
			FString::Printf(TEXT("Slow: %d"), RemainingTime)
		));
		SlowTimer->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SlowTimer->SetVisibility(ESlateVisibility::Hidden);
	}
}	