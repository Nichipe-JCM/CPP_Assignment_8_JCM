// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "CoinItem.generated.h"

/**
 * 
 */
UCLASS()
class CPP_ASSIGNMENT_8_JCM_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ACoinItem();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 PointValue;

	virtual void ActivateItem(AActor* Activator) override;
};
