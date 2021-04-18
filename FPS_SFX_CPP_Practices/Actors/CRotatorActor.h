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

	// ȸ�� 3����, �⺻������ �� �ʱ�ȭ �� �ϸ� 0�̴�.
	// ������
	UPROPERTY(EditAnywhere)
		float Pitch;

	// ������
	UPROPERTY(EditAnywhere)
		float Yaw;

	// ������
	UPROPERTY(EditAnywhere)
		float Roll;
	
};
