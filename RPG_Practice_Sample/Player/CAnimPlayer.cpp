// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimPlayer.h"
#include "CPlayerBase.h"

UCAnimPlayer::UCAnimPlayer()
{

}

void UCAnimPlayer::NativeUpdateAnimation(float DeltaSeconds)
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn == NULL)
		return;
	FVector vec = pawn->GetVelocity();
	vec.Z = 0;
	Velocity = vec.Size();
}
