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
	currentSpeed = GetActorForwardVector() * amount * speedMultiplier;
	forwardSpeed = currentSpeed;
	FloatingPawnMovement->AddInputVector(currentSpeed);
}

void AKikiFlight::MoveRight(float amount)
{
	sideSpeed = GetActorRightVector() * amount;
	FloatingPawnMovement->AddInputVector(GetActorRightVector() * amount);
}

void AKikiFlight::Turn(float amount)
{
	AddControllerYawInput(amount);

	AddControllerRollInput(amount);

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("roll value: %f,  amount: %f"), resetRotationValue, amount));

	isRolling = (bool)FMath::Abs(amount);


	if (amount > 0.0f)
		resetRotationValue = -1.0f;
	else if (amount < 0.0f)
		resetRotationValue = 1.0f;
}

void AKikiFlight::LookUp(float amount)
{
	AddControllerPitchInput(amount);
}

// Called every frame
void AKikiFlight::Tick(float DeltaTime)
{
	const float resetRotationMagnitude = 125.0f;
	// Reset the rotation when not rolling
	if (!isRolling && GetControlRotation().Euler().X > 5.0f)
			AddControllerRollInput(resetRotationMagnitude * resetRotationValue * DeltaTime);
	
	Super::Tick(DeltaTime);
}

void AKikiFlight::NotifyHit(UPrimitiveComponent* myComp, AActor* other, UPrimitiveComponent* otherComp, bool selfMoved, FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hit)
{
	Super::NotifyHit(myComp, other, otherComp, selfMoved, hitLocation, hitNormal, normalImpulse, hit);

	// Deflect before colliding
	AddControllerPitchInput(hitNormal.Size() * -GetWorld()->GetTimeSeconds()/10.0f);

	const float deflectVal = 200.0f;

	FloatingPawnMovement->AddInputVector(hitNormal * deflectVal);
}

// Called to bind functionality to input
void AKikiFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Input
	PlayerInputComponent->BindAxis("MoveForward", this, &AKikiFlight::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKikiFlight::MoveRight);
	PlayerInputComponent->BindAxis("LookRight", this, &AKikiFlight::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AKikiFlight::LookUp);
}

