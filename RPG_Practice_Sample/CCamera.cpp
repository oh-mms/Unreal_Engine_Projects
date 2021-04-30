// Fill out your copyright notice in the Description page of Project Settings.


#include "CCamera.h"
#include "Player/CPlayerBase.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACCamera::ACCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LerpSpeed = 10;
}

// Called when the game starts or when spawned
void ACCamera::BeginPlay()
{
	Super::BeginPlay();

	if (Camera == NULL)
	{
		for (TActorIterator<ACameraActor> iter(GetWorld()); iter; ++iter)
		{
			if (iter->GetName() == "MainCamera")
				Camera = *iter;
		}
	}

	APlayerController* cont = GetWorld()->GetFirstPlayerController();
	if (cont == NULL)
		return;

	cont->SetViewTarget(Camera);


	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	FVector playerLocation = player->GetActorLocation();
	FVector cameraLocation = Camera->GetActorLocation();

	Offset = cameraLocation - playerLocation;
}

// Called every frame
void ACCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	FVector playerLocation = player->GetActorLocation();
	FRotator playerRotation = player->GetActorRotation();
	FVector forward = player->GetActorForwardVector();

	FVector newLocation = playerLocation + Offset;
	FVector cameraLocation = Camera->GetActorLocation();

	newLocation = FMath::Lerp(cameraLocation, newLocation, DeltaTime * LerpSpeed);

	Camera->SetActorLocation(newLocation);
	Camera->SetActorRotation(playerRotation);
}

