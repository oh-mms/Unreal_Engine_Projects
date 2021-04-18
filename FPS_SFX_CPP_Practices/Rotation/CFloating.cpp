// Fill out your copyright notice in the Description page of Project Settings.

#include "CFloating.h"


// Sets default values
ACFloating::ACFloating()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACFloating::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACFloating::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = GetActorLocation();

	// DeltaTime은 0~1 사이의 값이므로, Sin을 취해봤자 계속 해당 구간에서만 값이 나올 것임.
	// 따라서 RunningTime을 더한 값으로 전 범위의 Sin값을 구하고, RunningTime만을 빼서
	// 의도한 바 처럼 전 범위에서의 DeltaTime에 대한 Sin 값을 구한다.
	float height = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	newLocation.X += height * Value.X;
	newLocation.Y += height * Value.Y;
	newLocation.Z += height * Value.Z;

	RunningTime += DeltaTime;

	SetActorLocation(newLocation);
}

