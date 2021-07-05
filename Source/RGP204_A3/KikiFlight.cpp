// Fill out your copyright notice in the Description page of Project Settings.


#include "KikiFlight.h"

#include <GameFramework/FloatingPawnMovement.h>

// Sets default values
AKikiFlight::AKikiFlight()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;
}

// Called when the game starts or when spawned
void AKikiFlight::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKikiFlight::MoveForward(float amount)
{
	forwardSpeedVal = amount;
	forwardSpeed = GetActorForwardVector() * forwardSpeedVal * forwardSpeedMultiplier;

	FloatingPawnMovement->AddInputVector(forwardSpeed);
}

void AKikiFlight::MoveRight(float amount)
{
	sideSpeedVal = amount;
	sideSpeed = GetActorRightVector() * sideSpeedVal * sideSpeedMultiplier;

	FloatingPawnMovement->AddInputVector(sideSpeed);
}

void AKikiFlight::Turn(float amount)
{
	// Add rotation
	AddControllerYawInput(amount);
	AddControllerRollInput(amount);

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("side value: %f,  forward val: %f"), sideSpeedVal, forwardSpeedVal));

	// Check if player is currently rolling
	isRolling = (bool)FMath::Abs(amount);

	// Check which way to roll to reset its x-orientation
	if (amount > 0.0f && (GetControlRotation().Euler().X < 90.0f && GetControlRotation().Euler().X >=0.02f))
		resetRotationValue = -1.0f;
	else if (amount < 0.0f && (GetControlRotation().Euler().X >= 270))
		resetRotationValue = 1.0f;
}

void AKikiFlight::LookUp(float amount)
{
	AddControllerPitchInput(amount);
}

void AKikiFlight::MoveDiagonal()
{
	// Strafe Speed Horizontally
	strafeSideSpeed = (GetActorForwardVector() * forwardSpeedVal) + (GetActorRightVector() * sideSpeedVal);
	if (forwardRightStrafe) { FloatingPawnMovement->AddInputVector(strafeSideSpeed * GetWorld()->GetTimeSeconds()); }

	// Strafe Up - forward speed 
	strafeUpForwardSpeed = (GetActorForwardVector() * forwardSpeedVal) + (GetActorUpVector() * upSpeedVal);
	if (upForwardStrafe) { FloatingPawnMovement->AddInputVector(strafeUpForwardSpeed * GetWorld()->GetTimeSeconds()); }

	// Strafe Up - Right speed
	strafeUpRightSpeed = (GetActorRightVector() * sideSpeedVal) + (GetActorUpVector() * upSpeedVal);
	if (upRightStrafe) { FloatingPawnMovement->AddInputVector(strafeUpRightSpeed * GetWorld()->GetTimeSeconds()); }

	// Strafe Up - Forward - Right speed
	strafeUpForwardRightSpeed = (GetActorForwardVector() * forwardSpeedVal) + (GetActorRightVector() * sideSpeedVal) + (GetActorUpVector() * upSpeedVal);
	if (upForwardRightStrafe) { FloatingPawnMovement->AddInputVector(strafeUpForwardRightSpeed * GetWorld()->GetTimeSeconds()); }
}

void AKikiFlight::MoveUp(float amount)
{
	// Kiki can fly vertically fairly quickly so the multiplier is applied to the amount directly
	upSpeedVal = amount * upSpeedMultiplier;
	FloatingPawnMovement->AddInputVector(GetActorUpVector() * amount);
}

// Called every frame
void AKikiFlight::Tick(float DeltaTime)
{
	// Reset the rotation when not rolling
	const float resetRotationMagnitude = 125.0f;
	if (!isRolling && GetControlRotation().Euler().X > 5.0f)
		AddControllerRollInput(resetRotationMagnitude * resetRotationValue * DeltaTime);

	// Check if player is strafing and which direction player is strafing
	// Forward-Right strafe
	if (forwardSpeedVal != 0.0f && sideSpeedVal != 0.0f && upSpeedVal == 0.0f) { forwardRightStrafe = true; }
	else { forwardRightStrafe = false; }

	// Up-Forward strafe
	if (forwardSpeedVal != 0.0f && sideSpeedVal == 0.0f && upSpeedVal != 0.0f) { upForwardStrafe = true; }
	else { upForwardStrafe = false; }

	// Up-Right strafe
	if (forwardSpeedVal == 0.0f && sideSpeedVal != 0.0f && upSpeedVal != 0.0f) { upRightStrafe = true; }
	else { upRightStrafe = false; }

	// Up-Forward-Right strafe
	if (forwardSpeedVal != 0.0f && sideSpeedVal != 0.0f && upSpeedVal != 0.0f) { upForwardRightStrafe = true; }
	else { upForwardRightStrafe = false; }

	// Apply diagonal movement
	this->MoveDiagonal();

	Super::Tick(DeltaTime);
}

//void AKikiFlight::NotifyHit(UPrimitiveComponent* myComp, AActor* other, UPrimitiveComponent* otherComp, bool selfMoved, FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hit)
//{
//	Super::NotifyHit(myComp, other, otherComp, selfMoved, hitLocation, hitNormal, normalImpulse, hit);
//
//	// Deflect before colliding
//	AddControllerPitchInput(hitNormal.Size() * -GetWorld()->GetTimeSeconds()/10.0f);
//
//	// Deflect the player
//	const float deflectVal = 200.0f;
//	FloatingPawnMovement->AddInputVector(hitNormal * deflectVal);	
//}

// Called to bind functionality to input
void AKikiFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Input
	PlayerInputComponent->BindAxis("MoveForward", this, &AKikiFlight::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKikiFlight::MoveRight);
	PlayerInputComponent->BindAxis("LookRight", this, &AKikiFlight::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AKikiFlight::LookUp);
	PlayerInputComponent->BindAxis("MoveUp", this, &AKikiFlight::MoveUp);
}
