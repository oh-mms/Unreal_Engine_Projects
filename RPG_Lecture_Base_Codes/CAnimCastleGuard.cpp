// Fill out your copyright notice in the Description page of Project Settings.

#include "CAnimCastleGuard.h"

UCAnimCastleGuard::UCAnimCastleGuard()
{

}

void UCAnimCastleGuard::NativeUpdateAnimation(float DeltaSeconds)
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn == NULL)
		return;

	FVector vec = pawn->GetVelocity();
	Velocity = vec.Size();
}