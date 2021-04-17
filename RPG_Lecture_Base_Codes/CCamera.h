// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCamera.generated.h"

UCLASS()
class U05_CPP_API ACCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "MainCamera"))
		AActor* Camera;

	UPROPERTY(EditAnywhere)
		float LerpSpeed;

private:
	FVector Offset;
};
