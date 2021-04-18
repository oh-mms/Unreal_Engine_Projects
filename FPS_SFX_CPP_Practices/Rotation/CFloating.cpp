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

	// DeltaTime�� 0~1 ������ ���̹Ƿ�, Sin�� ���غ��� ��� �ش� ���������� ���� ���� ����.
	// ���� RunningTime�� ���� ������ �� ������ Sin���� ���ϰ�, RunningTime���� ����
	// �ǵ��� �� ó�� �� ���������� DeltaTime�� ���� Sin ���� ���Ѵ�.
	float height = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	newLocation.X += height * Value.X;
	newLocation.Y += height * Value.Y;
	newLocation.Z += height * Value.Z;

	RunningTime += DeltaTime;

	SetActorLocation(newLocation);
}

