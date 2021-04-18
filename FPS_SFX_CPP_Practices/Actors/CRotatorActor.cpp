// Fill out your copyright notice in the Description page of Project Settings.

#include "CRotatorActor.h"


// Sets default values
ACRotatorActor::ACRotatorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACRotatorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACRotatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// SetActorRotate�� ������ Set�� �� ��, AddActor�� ���� + ��� �� ����.
	FRotator rot = FRotator(Pitch, Yaw, Roll);
	//AddActorLocalRotation(rot);

	FQuat quat = FQuat(rot);
	SetActorRotation(quat);
}

