// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimGriffon.generated.h"

/**
 * 
 */
UCLASS()
class SINGLERPG_API UCAnimGriffon : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCAnimGriffon();

protected:
	void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadOnly)
		float Velocity;
};
