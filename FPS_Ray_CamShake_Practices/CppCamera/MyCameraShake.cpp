// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraShake.h"


UMyCameraShake::UMyCameraShake()
{
	OscillationDuration = 0.25f; // 파동(흔드는) 시간
	OscillationBlendInTime = 0.05f; // 강할수록 진동이 세짐.
	OscillationBlendOutTime = 0.05f; // 강할수록 빨리 꺼짐.

	RotOscillation.Pitch.Amplitude = FMath::RandRange(5.0f, 10.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(25.0f, 35.0f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(5.0f, 10.0f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(25.0f, 35.0f);
}

