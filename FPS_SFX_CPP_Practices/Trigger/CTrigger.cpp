// Fill out your copyright notice in the Description page of Project Settings.

#include "CTrigger.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/StaticMeshActor.h"
#include "ConstructorHelpers.h"
#include "CRadialImpulse.h"
#include "Misc/OutputDeviceNull.h"


// Sets default values
ACTrigger::ACTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnActorBeginOverlap.AddDynamic(this, &ACTrigger::OnBegin);
	OnActorEndOverlap.AddDynamic(this, &ACTrigger::OnEnd);

	ConstructorHelpers::FClassFinder<ACRadialImpulse> radialImpulse
	(
		// Blueprint에서 불러올 것 임을 명시. 맨 뒤에는 _C를 붙여 C로부터 만들어진 것임을 명시.
		TEXT("Blueprint'/Game/Blueprints/BpCRadialImpulse.BpCRadialImpulse_C'")
	);

	if (radialImpulse.Succeeded())
		RadialImpulseClass = radialImpulse.Class;
}

// Called when the game starts or when spawned
void ACTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	DrawDebugBox
	(
		GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
		FColor::Purple, true, FLT_MAX, 0, 3
	);
}

// Called every frame
void ACTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACTrigger::OnBegin(AActor * actor, AActor * otherActor)
{
	AActor* player = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (otherActor != NULL && otherActor != this && player == otherActor)
	{
		//FString str = "Create";

		//FOutputDeviceNull ar;
		//this->CallFunctionByNameWithArguments(*str, ar, NULL, true);

		if (RadialImpulse == NULL)
		{
			FTransform transform = FTransform();
			transform.SetLocation(FVector(1100, -320, 170));

			RadialImpulse = GetWorld()->SpawnActor<ACRadialImpulse>
			(
				RadialImpulseClass, transform, FActorSpawnParameters()
			);
		}
	}
}

void ACTrigger::OnEnd(AActor * actor, AActor * otherActor)
{
	AActor* player = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (otherActor != NULL && otherActor != this && player == otherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("End : %s"), *otherActor->GetName());
	}
}
