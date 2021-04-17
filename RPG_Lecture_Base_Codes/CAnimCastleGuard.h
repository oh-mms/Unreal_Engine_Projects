// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimCastleGuard.generated.h"

/**
 * 
 */
UCLASS()
class U05_CPP_API UCAnimCastleGuard : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCAnimCastleGuard();

protected:
	void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadOnly)
		float Velocity;
};
