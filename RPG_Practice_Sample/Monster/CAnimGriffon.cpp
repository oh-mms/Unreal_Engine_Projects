// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimGriffon.h"

UCAnimGriffon::UCAnimGriffon()
{

}

void UCAnimGriffon::NativeUpdateAnimation(float DeltaSeconds)
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn == NULL)
		return;

	FVector vec = pawn->GetVelocity();
	Velocity = vec.Size();
}
