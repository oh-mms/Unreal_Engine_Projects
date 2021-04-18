// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRotationAxis.generated.h"

UCLASS()
class U14_CPP_API ACRotationAxis : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACRotationAxis();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
public:
	UPROPERTY(EditAnywhere, Category = "Rotation")
		float AngleAxis;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		FVector Dimensions;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		FVector AxisVector;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		float Multiplier;

private:
	void Rotation(float DeltaTime);
	void Rotation2(float DeltaTime);
	void Rotation3(float DeltaTime);
};
