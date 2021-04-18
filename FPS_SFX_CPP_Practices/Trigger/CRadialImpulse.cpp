// Fill out your copyright notice in the Description page of Project Settings.

#include "CRadialImpulse.h"
#include "WorldCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ACRadialImpulse::ACRadialImpulse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACRadialImpulse::BeginPlay()
{
	Super::BeginPlay();
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), 500.0f, 50, FColor::Green, true);
}

// Called every frame
void ACRadialImpulse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACRadialImpulse::Explosion()
{
	TArray<FHitResult> OutHits;

	FVector location = GetActorLocation();

	FVector start = location;
	FVector end = location;

	FCollisionShape collision = FCollisionShape::MakeSphere(500.0f);
	
	bool bHit = GetWorld()->SweepMultiByChannel(OutHits, start, end, FQuat::Identity, 
		ECC_WorldStatic, collision);

	if (bHit == true)
	{
		for (FHitResult& result : OutHits)
		{
			UStaticMeshComponent* comp = Cast<UStaticMeshComponent>(result.GetActor()->GetRootComponent());
			
			if (comp != NULL)
			{
				comp->AddRadialImpulse(GetActorLocation(), 500.0f, 1000.0f, ERadialImpulseFalloff::RIF_Constant, true);
			}
		}
	}
}

