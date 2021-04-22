// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItemActor.generated.h"

UCLASS()
class U14_CPP_API ACItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mesh;

	// 얘를 잡고있는 컴포넌트
	UPROPERTY(EditAnywhere)
		USceneComponent* Holding;

	UFUNCTION()
		void RotateActor();

	UFUNCTION()
		void Pickup();

private:
	bool bHolding;
	bool bGravity;

	FRotator ControlRotation;
	class ACharacter* Player;
	class UCameraComponent* PlayerCamera;

	FVector ForwardVector;
};
