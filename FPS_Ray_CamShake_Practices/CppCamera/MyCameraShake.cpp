// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraShake.h"


UMyCameraShake::UMyCameraShake()
{
	OscillationDuration = 0.25f; // �ĵ�(����) �ð�
	OscillationBlendInTime = 0.05f; // ���Ҽ��� ������ ����.
	OscillationBlendOutTime = 0.05f; // ���Ҽ��� ���� ����.

	RotOscillation.Pitch.Amplitude = FMath::RandRange(5.0f, 10.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(25.0f, 35.0f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(5.0f, 10.0f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(25.0f, 35.0f);
}

