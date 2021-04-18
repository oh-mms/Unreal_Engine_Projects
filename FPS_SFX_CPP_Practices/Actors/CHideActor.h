// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CHideActor.generated.h"

UCLASS()
class U14_CPP_API ACHideActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACHideActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere, Category = "Disable")
		bool HideInGame;

	UFUNCTION(BlueprintCallable)
		void DisableActor(bool bHide);

private:
	class UStaticMeshComponent* cylinder;
};
