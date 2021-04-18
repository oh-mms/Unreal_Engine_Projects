// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCameraChanger.generated.h"

UCLASS()
class U14_CPP_API ACCameraChanger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCameraChanger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		AActor* Camera[2];
	
private:
	float TimeTo;

	const float TimeBetween = 2.0f;
	const float SmoothBlendTime = 0.75f;
};
