// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnClass.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "PawnController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "SpartaGameState.h"

// Sets default values
APawnClass::APawnClass()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetSimulatePhysics(false);

	PawnMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PawnMesh"));
	PawnMesh->SetupAttachment(RootComponent);
	PawnMesh->SetSimulatePhysics(false);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	OverHeadWidget->SetupAttachment(PawnMesh);
	OverHeadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PawnMeshAsset(TEXT("/Game/Resources/Characters/Meshes/SKM_Manny.SKM_Manny"));
	if (PawnMeshAsset.Succeeded())
	{
		PawnMesh->SetSkeletalMesh(PawnMeshAsset.Object);
	}

	MaxHealth = 100.0f;
	Health = MaxHealth;

}

// Called when the game starts or when spawned
void APawnClass::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverHeadHP();
}

// Called every frame
void APawnClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	float LineTraceLength = CapsuleComponent->GetScaledCapsuleHalfHeight() + 5.0f;

	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.0f, 0.0f, LineTraceLength);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,
		Start,
		End,
		ECC_Visibility,
		CollisionParams
	);

	if (bHit && VerticalVelocity <= 0.0f)
	{
		VerticalVelocity = 0.0f;
		isGrounded = true;
		float FloorZ = HitResult.ImpactPoint.Z + CapsuleComponent->GetScaledCapsuleHalfHeight() + KINDA_SMALL_NUMBER;
		FVector CurrentLocation = GetActorLocation();
		if (FMath::Abs(CurrentLocation.Z - FloorZ) > KINDA_SMALL_NUMBER)
		{
			CurrentLocation.Z = FloorZ;
			SetActorLocation(CurrentLocation, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
	else isGrounded = false;

	if (!isGrounded)
	{
		VerticalVelocity -= GravityStrength * DeltaTime;
		FVector GravityMovement(0.0f, 0.0f, VerticalVelocity * DeltaTime);
		AddActorWorldOffset(GravityMovement, true);
	}

}

// Called to bind functionality to input
void APawnClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (APawnController* PawnController = Cast<APawnController>(GetController()))
		{
			if (PawnController->MoveAction)
			{
				EnhancedInput->BindAction(
					PawnController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&APawnClass::Move
				);
			}
			if (PawnController->LookAction)
			{
				EnhancedInput->BindAction(
					PawnController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&APawnClass::Look
				);
			}
			if (PawnController->JumpAction)
			{
				EnhancedInput->BindAction(
					PawnController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&APawnClass::StartJump
				);
			}
		}
	}
}

void APawnClass::Move(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if(InputVector.IsZero())
	{
		return;
	}

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float ScaledMoveSpeed;
	if(isGrounded)
	{
		ScaledMoveSpeed = MoveSpeed * DeltaTime * SpeedMultiplier;
	}
	else
	{
		ScaledMoveSpeed = MoveSpeed * 1.2f * DeltaTime * SpeedMultiplier;
	}

	FVector DeltaLocation(InputVector.X * ScaledMoveSpeed, InputVector.Y * ScaledMoveSpeed, 0.0f);
	AddActorLocalOffset(DeltaLocation, true);
}

void APawnClass::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();
	if(LookInput.IsZero())
	{
		return;
	}
	FRotator DeltaRotation(0.0f, LookInput.X * LookSensitivity, 0.0f);
	AddActorLocalRotation(DeltaRotation);

	if (SpringArm)
	{
		FRotator ArmRotation = SpringArm->GetRelativeRotation();
		ArmRotation.Pitch += LookInput.Y * LookSensitivity * 1.2f;
		ArmRotation.Pitch = FMath::Clamp(ArmRotation.Pitch, -80.0f, 60.0f);
		SpringArm->SetRelativeRotation(ArmRotation);
	}
}

void APawnClass::StartJump(const FInputActionValue& Value)
{
	if (isGrounded)
	{
		VerticalVelocity = JumpStrength;
		isGrounded = false;
	}
}

void APawnClass::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health increased to: %f"), Health);
	UpdateOverHeadHP();
}

void APawnClass::SetSpeedMultiplier(float Multiplier)
{
	SpeedMultiplier = Multiplier;
}

float APawnClass::TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);

	if (Health <= 0.0f)
	{
		OnDeath();
	}
	UpdateOverHeadHP();

	return ActualDamage;
}

void APawnClass::OnDeath()
{
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

int32 APawnClass::GetHealth() const
{
	return Health;
}

void APawnClass::UpdateOverHeadHP()
{
	if (!OverHeadWidget) return;	

	UUserWidget* OverHeadWidgetInstance = OverHeadWidget->GetUserWidgetObject();
	if (!OverHeadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverHeadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
}

void APawnClass::ApplyPoison(float Duration, float Damage)
{
	GetWorldTimerManager().ClearTimer(PoisonTimerHandle);

	RemainingPoisonTicks = FMath::RoundToInt(Duration);
	CurrentPoisonDamage = Damage;

	if (APawnController* PC = Cast<APawnController>(GetController()))
	{
		PC->PoisonHUD(RemainingPoisonTicks, true);
	}

	GetWorldTimerManager().SetTimer(
		PoisonTimerHandle,
		this,
		&APawnClass::PoisonTick,
		1.0f,
		true);
}

void APawnClass::PoisonTick()
{
	if (RemainingPoisonTicks > 0)
	{
		UGameplayStatics::ApplyDamage(
			this,
			CurrentPoisonDamage,
			nullptr,
			nullptr,
			UDamageType::StaticClass());
		RemainingPoisonTicks--;

		if (APawnController* PC = Cast<APawnController>(GetController()))
		{
			PC->PoisonHUD(RemainingPoisonTicks, true);
		}
	}

	if (RemainingPoisonTicks <= 0)
	{
		GetWorldTimerManager().ClearTimer(PoisonTimerHandle);
		if (APawnController* PC = Cast<APawnController>(GetController()))
		{
			PC->PoisonHUD(0, false);
		}
	}
}

void APawnClass::ApplySlow(float Duration, float SlowMultiplier)
{
	GetWorldTimerManager().ClearTimer(SlowTimerHandle);

	RemainingSlowTicks = FMath::RoundToInt(Duration);
	SetSpeedMultiplier(SlowMultiplier);

	if (APawnController* PC = Cast<APawnController>(GetController()))
	{
		PC->SlowHUD(RemainingSlowTicks, true);
	}
	GetWorldTimerManager().SetTimer(
		SlowTimerHandle,
		this,
		&APawnClass::SlowTick,
		1.0f,
		true);
}

void APawnClass::SlowTick()
{
	if (RemainingSlowTicks > 0)
	{
		RemainingSlowTicks--;
		if (APawnController* PC = Cast<APawnController>(GetController()))
		{
			PC->SlowHUD(RemainingSlowTicks, true);
		}
	}
	if (RemainingSlowTicks <= 0)
	{
		GetWorldTimerManager().ClearTimer(SlowTimerHandle);
		SetSpeedMultiplier(1.0f);
		if (APawnController* PC = Cast<APawnController>(GetController()))
		{
			PC->SlowHUD(0, false);
		}
	}
}