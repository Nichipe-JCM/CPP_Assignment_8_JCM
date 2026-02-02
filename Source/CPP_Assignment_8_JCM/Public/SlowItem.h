// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SlowItem.generated.h"

/**
 * 
 */
UCLASS()
class CPP_ASSIGNMENT_8_JCM_API ASlowItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ASlowItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow")
	float SlowSpeedMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow")
	int32 TotalSlowDuration;

	int32 TickCount;
	FTimerHandle SlowTimerHandle;

	virtual void ActivateItem(AActor* Activator) override;

	void UpdateSlowHUD(AActor* TargetActor);
};
