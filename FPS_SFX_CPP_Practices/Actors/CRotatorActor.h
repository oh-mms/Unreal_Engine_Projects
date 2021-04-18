// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRotatorActor.generated.h"

UCLASS()
class U14_CPP_API ACRotatorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACRotatorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 회전 3가지, 기본적으로 값 초기화 안 하면 0이다.
	// 가로축
	UPROPERTY(EditAnywhere)
		float Pitch;

	// 수직축
	UPROPERTY(EditAnywhere)
		float Yaw;

	// 전방축
	UPROPERTY(EditAnywhere)
		float Roll;
	
};
