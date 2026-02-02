// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	ItemDataTable = nullptr;
}

AActor* ASpawnVolume::SpawnRandomItem(int32 CurrentWave)
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem(CurrentWave))
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

FItemSpawnRow* ASpawnVolume::GetRandomItem(int32 CurrentWave) const
{
	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> FilteredRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	
	TArray<FItemSpawnRow*> AllRows;
	ItemDataTable->GetAllRows(ContextString, AllRows);

	for (FItemSpawnRow* Row : AllRows)
	{
		if (Row && Row->Wave == CurrentWave)
		{
			FilteredRows.Add(Row);
		}
	}

	if (FilteredRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (FItemSpawnRow* Row : FilteredRows)
	{
		if(Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulatedChance = 0.0f;

	for (FItemSpawnRow* Row : FilteredRows)
	{
		AccumulatedChance += Row->SpawnChance;
		if (RandValue <= AccumulatedChance)
		{
			return Row;
		}
	}
	
	return nullptr;
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;
	
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);
	if (SpawnedActor) return SpawnedActor;
	return nullptr;
}