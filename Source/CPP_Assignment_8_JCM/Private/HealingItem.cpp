// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "PawnClass.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if(Activator && Activator->ActorHasTag("Player"))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.f,
				FColor::Green,
				FString::Printf(TEXT("Healed %d health!"), HealAmount)
			);

		if (APawnClass* PlayerCharacter = Cast<APawnClass>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}

		DestroyItem();
	}
}
