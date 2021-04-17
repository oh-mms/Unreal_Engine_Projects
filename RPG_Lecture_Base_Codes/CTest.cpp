// Fill out your copyright notice in the Description page of Project Settings.

#include "CTest.h"
#include "Engine.h"


// Sets default values
ACTest::ACTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACTest::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Test");
}

// Called every frame
void ACTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

