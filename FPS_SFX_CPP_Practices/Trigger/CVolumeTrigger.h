// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CVolumeTrigger.generated.h"

UCLASS()
class U14_CPP_API ACVolumeTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACVolumeTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
public:
	UPROPERTY(VisibleAnywhere, Category = "Switch")
		class UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere, Category = "Switch")
		float LightIntensity;

	UPROPERTY(VisibleAnywhere, Category = "Switch")
		class USphereComponent* Sphere;

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
	void Toggle();
};
