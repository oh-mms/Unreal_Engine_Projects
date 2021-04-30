// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Player/CPlayerBase.h"
#include "Sword.generated.h"

UCLASS()
class SINGLERPG_API ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class ACPlayerBase* MyPawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class USkeletalMeshComponent* SwordMesh;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void SetOwingPawn(ACPlayerBase* NewOwner);

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		class UCapsuleComponent* SwordCollision;
};
