// Fill out your copyright notice in the Description page of Project Settings.

#include "CBillboard.h"
#include "Components/BillboardComponent.h"


// Sets default values
ACBillboard::ACBillboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(FName("Bill"));
	RootComponent = Billboard;
}

// Called when the game starts or when spawned
void ACBillboard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACBillboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

