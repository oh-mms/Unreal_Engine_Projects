// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimBarghest.generated.h"

/**
 * 
 */
UCLASS()
class SINGLERPG_API UCAnimBarghest : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCAnimBarghest();

protected:
	void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadOnly)
		float Velocity;
};
