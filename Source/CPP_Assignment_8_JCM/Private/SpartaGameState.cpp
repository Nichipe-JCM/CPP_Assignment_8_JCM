// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "SpartaGameInstance.h"
#include "PawnController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerStart.h"
#include "PawnClass.h"
#include "Drone.h"
#include "GameFramework/SpringArmComponent.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	MaxLevels = 3;
	MaxWaves = 3;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		return;
	}
	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if(UGameInstance* GameInstance = GetGameInstance())
	{
		if(USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			if (SpartaGameInstance)
			{
				SpartaGameInstance->AddToScore(Amount);
			}
		}
	}
	Score += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

void ASpartaGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APawnController* PawnController = Cast<APawnController>(PlayerController))
		{
			PawnController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			if (SpartaGameInstance)
			{
				CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
			}
		}
	}

	CurrentWaveIndex = 0;

	StartWave();
}

void ASpartaGameState::StartWave()
{

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40 + CurrentLevelIndex * 10;
	LevelDuration = 60.0f - CurrentWaveIndex * 10.0f;

	for (int32 i = 0; i < ItemToSpawn; ++i)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem(CurrentWaveIndex + 1);
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.0f,
			FColor::Green,
			FString::Printf(TEXT("Wave %d started! Collect all %d coins in %.1f seconds."),
				CurrentWaveIndex + 1,
				SpawnedCoinCount,
				LevelDuration)
		);
		if (CurrentLevelIndex == 0 && CurrentWaveIndex == 1)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.0f,
				FColor::Yellow,
				TEXT("Poison potion appeared! Avoid purple potions!")
			);
		}
		if (CurrentLevelIndex == 0 && CurrentWaveIndex == 2)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.0f,
				FColor::Yellow,
				TEXT("Slow potion appeared! Avoid yellow potions!")
			);
		}
	}

	AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	APawnController* PawnController = Cast<APawnController>(GetWorld()->GetFirstPlayerController());
	if (PawnController && PlayerStart)
	{
		APawn* PlayerPawn = PawnController->GetPawn();
		if (PlayerPawn)
		{
			PlayerPawn->SetActorLocation(PlayerStart->GetActorLocation());
			PlayerPawn->SetActorRotation(PlayerStart->GetActorRotation());
		}
		PawnController->SetControlRotation(PlayerStart->GetActorRotation());
		if (APawnClass* Pawn = Cast<APawnClass>(PlayerPawn))
		{
			if (Pawn->SpringArm)
			{
				Pawn->SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
			}
		}
		else if (ADrone* DronePawn = Cast<ADrone>(PlayerPawn))
		{
			if (DronePawn->SpringArm)
			{
				DronePawn->SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
			}
		}
	}
}

void ASpartaGameState::EndWave()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	if (CollectedCoinCount < SpawnedCoinCount)
	{
		OnGameOver();
	}
	else 	
	{
		CurrentWaveIndex++;
		if (CurrentWaveIndex < MaxWaves)
		{
			StartWave();
		}
		else
		{
			EndLevel();
		}
	}
}

void ASpartaGameState::OnLevelTimeUp()
{
	EndWave();
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	UE_LOG(LogTemp, Warning, TEXT("Coins collected: %d/%d"),
		CollectedCoinCount,
		SpawnedCoinCount
	);

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndWave();
	}
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			if (SpartaGameInstance)
			{
				AddScore(Score);
				CurrentLevelIndex++;
				SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;

				if (CurrentLevelIndex >= MaxLevels)
				{
					OnGameOver();
					return;
				}

				if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
				{
					UGameplayStatics::OpenLevel(
						GetWorld(),
						LevelMapNames[CurrentLevelIndex]
					);
				}
				else
				{
					OnGameOver();
				}
			}
		}
	}

	
}

void ASpartaGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APawnController* PawnController = Cast<APawnController>(PlayerController))
		{
			PawnController->SetPause(true);
			PawnController->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APawnController* PawnController = Cast<APawnController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = PawnController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeValue"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreValue"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
						if (SpartaGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}

				if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d/%d"), CurrentWaveIndex + 1, MaxWaves)));
				}
			}
		}
	}
}