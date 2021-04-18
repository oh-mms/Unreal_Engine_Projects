// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTrigger.generated.h"

UCLASS()
class U14_CPP_API ACTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void OnBegin(AActor* actor, AActor* otherActor);

	UFUNCTION()
		void OnEnd(AActor* actor, AActor* otherActor);

	UPROPERTY()
		TSubclassOf<class ACRadialImpulse> RadialImpulseClass;

public:
	class ACRadialImpulse* RadialImpulse;
};
