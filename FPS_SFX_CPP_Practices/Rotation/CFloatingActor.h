// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "CFloatingActor.generated.h"

UCLASS()
class U14_CPP_API ACFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACFloatingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:
	FVector InitLocation;
	FVector TargetLocation;

	FTimeline Timeline;

	UFUNCTION()
		void HandleProgress(float Value);

public:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		float MaxBounceHeight;

	UPROPERTY(EditAnywhere)
		UCurveFloat* CurveFloat;
};
