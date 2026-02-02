// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowItem.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "PawnController.h"
#include "PawnClass.h"
#include "Drone.h"

ASlowItem::ASlowItem()
{
	SlowSpeedMultiplier = 0.5f;
	TotalSlowDuration = 5;
	ItemType = "Slow";
	TickCount = 0;
}

void ASlowItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (APawnClass* Player = Cast<APawnClass>(Activator))
	{
		Player->ApplySlow(TotalSlowDuration, SlowSpeedMultiplier);
	}
	else if (ADrone* Drone = Cast<ADrone>(Activator))
	{
		Drone->ApplySlow(TotalSlowDuration, SlowSpeedMultiplier);
	}

	DestroyItem();
}