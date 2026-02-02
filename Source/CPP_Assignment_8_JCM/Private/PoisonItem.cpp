// Fill out your copyright notice in the Description page of Project Settings.


#include "PoisonItem.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "PawnController.h"
#include "Components/SphereComponent.h"
#include "PawnClass.h"
#include "Drone.h"

APoisonItem::APoisonItem()
{
	PoisonDamagePerSecond = 5;
	TotalPoisonDuration = 5;
	ItemType = "Poison";
}

void APoisonItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (APawnClass* Player = Cast<APawnClass>(Activator))
	{
		Player->ApplyPoison(TotalPoisonDuration, PoisonDamagePerSecond);
	}
	else if (ADrone* Drone = Cast<ADrone>(Activator))
	{
		Drone->ApplyPoison(TotalPoisonDuration, PoisonDamagePerSecond);
	}
	DestroyItem();
}