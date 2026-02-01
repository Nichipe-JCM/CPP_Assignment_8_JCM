// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PawnController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class CPP_ASSIGNMENT_8_JCM_API APawnController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APawnController();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|Drone")
	TSubclassOf<APawn> DronePawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|Input")
	UInputMappingContext* PawnInputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|Input")
	UInputMappingContext* DroneInputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|Input")
	UInputMappingContext* GlobalInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|PawnInput")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|PawnInput")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|PawnInput")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|DroneInput")
	UInputAction* DroneMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|DroneInput")
	UInputAction* DroneLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|DroneInput")
	UInputAction* DroneAscendAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|DroneInput")
	UInputAction* DroneRollAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|DroneInput")
	UInputAction* DroneStableAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|Input")
	UInputAction* SwitchToPawnAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PawnController|Input")
	UInputAction* SwitchToDroneAction;

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void SwitchToPawn(const FInputActionValue& Value);

	void SwitchToDrone(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PawnController|HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnController|HUD")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PawnController|Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PawnController|Menu")
	UUserWidget* MainMenuWidgetInstance;

	UFUNCTION(BlueprintPure, Category = "PawnController|HUD")
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable, Category = "PawnController|Menu")
	void ShowGameHUD();
	UFUNCTION(BlueprintCallable, Category = "PawnController|Menu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "PawnController|Menu")
	void StartGame();



private:

	UPROPERTY() APawn* CharacterPawn;
	UPROPERTY() APawn* DronePawn;
};
