// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActionDoor.generated.h"

UCLASS()
class U14_CPP_API ACActionDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACActionDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:
	class UBoxComponent* CheckArea;
	class UStaticMeshComponent* Door;

public:
	void Toggle(FVector Forward);

private:
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

private:
	bool bOpening;
	bool bClosing;
	bool bClosed;

	float DotProduct;
	float AddRotation;
	float PosNegative; // 문의 앞/뒤
	float CurrentRotaiton; // 현재 회전값

	float MaxDegree; // 최대 문이 열릴 각도
};
