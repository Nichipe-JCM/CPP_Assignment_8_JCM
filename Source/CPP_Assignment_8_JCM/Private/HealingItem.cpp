// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "PawnClass.h"
#include "Drone.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if(Activator && Activator->ActorHasTag("Player"))
	{
		
		if (APawnClass* PlayerCharacter = Cast<APawnClass>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		else if (ADrone* Drone = Cast<ADrone>(Activator))
		{
			Drone->AddHealth(HealAmount);
		}

		DestroyItem();
	}
}
