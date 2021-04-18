// Fill out your copyright notice in the Description page of Project Settings.

#include "CHitComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ACHitComponent::ACHitComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));
	Box->SetSimulatePhysics(true);
	Box->SetNotifyRigidBodyCollision(true);

	Box->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	Box->OnComponentHit.AddDynamic(this, &ACHitComponent::OnHit);

	RootComponent = Box;
}

// Called when the game starts or when spawned
void ACHitComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACHitComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACHitComponent::OnHit_Implementation(UPrimitiveComponent * OverComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & SweepResult)
{
	if (OtherActor != NULL && OtherActor != this && OtherComp != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetFName().ToString());
	}
}

