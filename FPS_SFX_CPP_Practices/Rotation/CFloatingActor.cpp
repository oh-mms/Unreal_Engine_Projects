// Fill out your copyright notice in the Description page of Project Settings.

#include "CFloatingActor.h"


// Sets default values
ACFloatingActor::ACFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));

}

// Called when the game starts or when spawned
void ACFloatingActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (CurveFloat == NULL)
		return;

	FOnTimelineFloat Func; // �ð��� ���� �ڵ����� ���ϴ� �Լ�
	Func.BindUFunction(this, FName("HandleProgress"));

	Timeline.AddInterpFloat(CurveFloat, Func);
	Timeline.SetLooping(true);

	InitLocation = TargetLocation = GetActorLocation();
	TargetLocation.Z += MaxBounceHeight;

	Timeline.PlayFromStart(); // BeginPlay���� ���۵ǰڴٴ� �Ҹ�.
	Timeline.SetPlayRate(0.25f);
}

// Called every frame
void ACFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ACFloatingActor::HandleProgress(float Value)
{
	FVector location = FMath::Lerp(InitLocation, TargetLocation, Value);

	SetActorLocation(location);
}

