// Fill out your copyright notice in the Description page of Project Settings.

#include "CVolumeTrigger.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


// Sets default values
ACVolumeTrigger::ACVolumeTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightIntensity = 3000;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(FName("Point Light"));
	PointLight->Intensity = LightIntensity;
	PointLight->bVisible = true;
	PointLight->LightColor = FColor::Red;
	RootComponent = PointLight;

	Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	Sphere->InitSphereRadius(300.0f);
	Sphere->SetCollisionProfileName("Trigger");
	Sphere->SetupAttachment(RootComponent);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACVolumeTrigger::OnOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACVolumeTrigger::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ACVolumeTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), 500.0f, 50, FColor::Green, true, FLT_MAX);
}

// Called every frame
void ACVolumeTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACVolumeTrigger::OnOverlapBegin(UPrimitiveComponent * OverComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AActor* player = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();

	if (OtherActor != NULL && OtherActor != this && player == OtherActor)
		Toggle();
}

void ACVolumeTrigger::OnOverlapEnd(UPrimitiveComponent * OverComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AActor* player = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();

	if (OtherActor != NULL && OtherActor != this && player == OtherActor)
		Toggle();
}

void ACVolumeTrigger::Toggle()
{
	PointLight->ToggleVisibility();
}

