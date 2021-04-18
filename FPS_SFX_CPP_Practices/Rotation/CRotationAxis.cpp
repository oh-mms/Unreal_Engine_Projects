// Fill out your copyright notice in the Description page of Project Settings.

#include "CRotationAxis.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


// Sets default values
ACRotationAxis::ACRotationAxis()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Dimensions = FVector(300, 0, 0);
	AxisVector = FVector(0, 0, 1);
	Multiplier = 50.0f;
	AngleAxis = 0.0f;
}

// Called when the game starts or when spawned
void ACRotationAxis::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACRotationAxis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Rotation(DeltaTime);
	//Rotation2(DeltaTime);
	Rotation3(DeltaTime);
}

void ACRotationAxis::Rotation(float DeltaTime)
{
	FVector newLocation = FVector(0, 0, 800);

	AngleAxis += Multiplier * DeltaTime;
	if (AngleAxis >= 360)
		AngleAxis = 0.0f;

	//FVector actorLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	// Dimensions를 중심으로 AngleAxis(각도 값), AxisVector(회전 축 값)
	FVector rotationValue = Dimensions.RotateAngleAxis(AngleAxis, AxisVector);

	newLocation.X += rotationValue.X;
	newLocation.Y += rotationValue.Y;
	newLocation.Z += rotationValue.Z;

	SetActorLocation(newLocation);
}

void ACRotationAxis::Rotation2(float DeltaTime)
{
	FVector newLocation = FVector(0, 0, 200);
	FVector radius = FVector(200, 0, 0);
	AngleAxis++;
	if (AngleAxis > 360.0f)
		AngleAxis = 1.0f;
	
	FVector rotationValue = radius.RotateAngleAxis(AngleAxis, FVector(0,0,1));

	newLocation.X += rotationValue.X;
	newLocation.Y += rotationValue.Y;
	newLocation.Z += rotationValue.Z;

	SetActorLocation(newLocation);
}

void ACRotationAxis::Rotation3(float DeltaTime)
{
	AngleAxis += Multiplier * DeltaTime;
	if (AngleAxis >= 360)
		AngleAxis = 0.0f;

	FVector newLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector rotationValue = Dimensions.RotateAngleAxis(AngleAxis, AxisVector);

	newLocation.X += rotationValue.X;
	newLocation.Y += rotationValue.Y;
	newLocation.Z += rotationValue.Z;

	SetActorLocation(newLocation);
}

