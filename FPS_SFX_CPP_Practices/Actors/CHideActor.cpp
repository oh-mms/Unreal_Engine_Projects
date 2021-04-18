// Fill out your copyright notice in the Description page of Project Settings.

#include "CHideActor.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"


// Sets default values
ACHideActor::ACHideActor()
	: HideInGame(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cylinder = CreateDefaultSubobject<UStaticMeshComponent>(FName("MyCylinder"));
	cylinder->SetupAttachment(RootComponent);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshCylinder
	(
		TEXT("/Game/StarterContent/Shapes/Shape_Cylinder")
	);

	if (meshCylinder.Succeeded())
	{
		cylinder->SetStaticMesh(meshCylinder.Object);
		cylinder->SetRelativeLocation(FVector(0, 0, 0));
		cylinder->SetWorldScale3D(FVector(1));
	}
}

// Called when the game starts or when spawned
void ACHideActor::BeginPlay()
{
	Super::BeginPlay();

	if (HideInGame == true)
		DisableActor(HideInGame);
}

// Called every frame
void ACHideActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACHideActor::DisableActor(bool bHide)
{
	SetActorHiddenInGame(bHide);
	SetActorEnableCollision(!bHide);
	SetActorTickEnabled(!bHide);
}

