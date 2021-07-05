// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KikiFlight.generated.h"

UCLASS()
class RGP204_A3_API AKikiFlight : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKikiFlight();

	// Variables
	bool isRolling;

	// Which way is the player strafing
	UPROPERTY(VisibleAnywhere, Category = "Flight")
	bool forwardRightStrafe;
	UPROPERTY(VisibleAnywhere, Category = "Flight")
	bool upForwardStrafe;
	UPROPERTY(VisibleAnywhere, Category = "Flight")
	bool upRightStrafe;
	UPROPERTY(VisibleAnywhere, Category = "Flight")
	bool upForwardRightStrafe;

	// Controls the strafing
	FVector strafeSideSpeed;
	FVector strafeUpForwardSpeed;
	FVector strafeUpRightSpeed;
	FVector strafeUpForwardRightSpeed;
	FVector currentSpeed;
	FVector forwardSpeed;
	FVector sideSpeed;

	// Rotation values
	float currentTurnAmount;
	float resetRotationValue;

	// Directional speed values
	float forwardSpeedVal;
	float sideSpeedVal;
	float upSpeedVal;

	UPROPERTY(EditAnywhere, Category = "Flight")
	float forwardSpeedMultiplier = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Flight")
	float sideSpeedMultiplier = 800.0f;
	UPROPERTY(EditAnywhere, Category = "Flight")
	float upSpeedMultiplier = 100.0f;
protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Movement
	void MoveForward(float amount);
	void MoveRight(float amount);
	void MoveDiagonal();
	void MoveUp(float amount);
	void Turn(float amount);
	void LookUp(float amount);

	UPROPERTY(EditAnywhere, Category = "FloatingPawnMovement")
	class UFloatingPawnMovement* FloatingPawnMovement;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//virtual void NotifyHit(class UPrimitiveComponent* myComp, class AActor* other, class UPrimitiveComponent* otherComp,
	//	bool selfMoved, FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hit) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
