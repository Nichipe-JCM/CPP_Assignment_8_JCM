// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "PoisonItem.generated.h"

/**
 * 
 */
UCLASS()
class CPP_ASSIGNMENT_8_JCM_API APoisonItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	APoisonItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison")
	int32 PoisonDamagePerSecond;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison")
	int32 TotalPoisonDuration;

	virtual void ActivateItem(AActor* Activator) override;

};
