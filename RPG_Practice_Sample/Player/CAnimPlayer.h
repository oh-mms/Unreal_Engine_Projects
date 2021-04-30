// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SINGLERPG_API UCAnimPlayer : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCAnimPlayer();

protected:
	void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadOnly)
		float Velocity;

	//UPROPERTY(BlueprintReadOnly)
	//	bool Jumping;
};
