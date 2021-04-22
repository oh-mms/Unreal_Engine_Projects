// Fill out your copyright notice in the Description page of Project Settings.

#include "CItemActor.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "ConstructorHelpers.h"


// Sets default values
ACItemActor::ACItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetSimulatePhysics(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> statue
	(
		TEXT("/Game/StarterContent/Props/SM_Statue")
	);

	Mesh->SetStaticMesh(statue.Object);
	RootComponent = Mesh;

	bHolding = false;
	bGravity = true;
}

// Called when the game starts or when spawned
void ACItemActor::BeginPlay()
{
	Super::BeginPlay();
	
	Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	PlayerCamera = Player->FindComponentByClass<UCameraComponent>();

	TArray<USceneComponent *> components;
	Player->GetComponents(components);

	for (USceneComponent*& comp : components)
	{
		if (comp->GetName() == "HoldingComp")
			Holding = Cast<USceneComponent>(comp);
	}

}

// Called every frame
void ACItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHolding == true && Holding != NULL)
		SetActorLocationAndRotation(Holding->GetComponentLocation(), Holding->GetComponentRotation());

}

void ACItemActor::RotateActor()
{
	ControlRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	// 임의로 넣음.
	ControlRotation.Roll = 180.0f;
	SetActorRotation(ControlRotation);
}

void ACItemActor::Pickup()
{
	// 이미 물건을 갖고 있으면 다른 아이템은 못 갖게 하기 위해 반대로 처리해줌.
	bHolding = !bHolding;
	bGravity = !bGravity;

	Mesh->SetEnableGravity(bGravity);
	Mesh->SetSimulatePhysics(bHolding ? false : true);
	Mesh->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	if (bHolding == false)
	{
		ForwardVector = PlayerCamera->GetForwardVector();
		Mesh->AddForce(ForwardVector * 100000 * Mesh->GetMass());
	}
}

