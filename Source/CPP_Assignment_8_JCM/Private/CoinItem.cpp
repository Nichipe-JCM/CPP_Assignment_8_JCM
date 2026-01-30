// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"
#include "Engine/World.h"
#include "SpartaGameStateBase.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.f,
				FColor::Green,
				FString::Printf(TEXT("Collected %d points!"), PointValue)
			);
		if(UWorld* World = GetWorld()) 
		{
			if(ASpartaGameStateBase* GameState = World->GetGameState<ASpartaGameStateBase>())
			{
				GameState->AddScore(PointValue);
			}
		}
		DestroyItem();
	}
}
