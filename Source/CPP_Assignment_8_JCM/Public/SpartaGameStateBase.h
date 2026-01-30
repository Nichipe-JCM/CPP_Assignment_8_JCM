// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpartaGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class CPP_ASSIGNMENT_8_JCM_API ASpartaGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ASpartaGameStateBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Score")
	int32 Score;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
};
