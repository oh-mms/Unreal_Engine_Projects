// Fill out your copyright notice in the Description page of Project Settings.

#include "CCameraChanger.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


// Sets default values
ACCameraChanger::ACCameraChanger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TimeTo = TimeBetween;
}

// Called when the game starts or when spawned
void ACCameraChanger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACCameraChanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeTo -= DeltaTime;
	if (TimeTo <= 0.0f)
	{
		TimeTo += TimeBetween;

		// PlayerController를 가져오는 다른 방법은 GetWorld()->GetFirstPlayerController() 도 있음.
		APlayerController* cont = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (Camera[1] != NULL && cont->GetViewTarget() == Camera[0])
			cont->SetViewTargetWithBlend(Camera[1], SmoothBlendTime);
		else
			cont->SetViewTarget(Camera[0]);
	}
}

