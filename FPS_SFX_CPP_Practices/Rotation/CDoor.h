// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDoor.generated.h"

UCLASS()
class U14_CPP_API ACDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
public:
	UFUNCTION()
		void OnOverlapBegin
		(
			UPrimitiveComponent* OverComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	UFUNCTION()
		void OnOverlapEnd
		(
			UPrimitiveComponent* OverComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);

private:
	class UStaticMeshComponent* Door;
	class UBoxComponent* Box;

	bool bOpen;
	float RotationAngle;

	FRotator Rotation;
};
