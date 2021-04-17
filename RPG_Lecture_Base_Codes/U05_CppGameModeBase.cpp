// Fill out your copyright notice in the Description page of Project Settings.

#include "U05_CppGameModeBase.h"
#include "CPlayer.h"
#include "CMutant.h"
#include "CCastleGurad.h"
#include "CLog.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

AU05_CppGameModeBase::AU05_CppGameModeBase()
	: castleGuardCount (0)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AU05_CppGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ACPlayer> iter(GetWorld()); iter; ++iter)
		player = *iter;

	for (TActorIterator<ACCastleGurad> iter(GetWorld()); iter; ++iter)
		castleGuardCount++;
/* 플레이어를 찾는 아래 방법도 있음.
	ACharacter* temp = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ACPlayer* player = Cast<ACPlayer>(temp);
*/

}

void AU05_CppGameModeBase::Tick(float delta)
{
	Super::Tick(delta);

	PlayerHpRatio = player->Hp / 100.0f;
}

void AU05_CppGameModeBase::SpawnBoss()
{
	/* 임시
	FVector location(-4220.0f, 4780.f, -90.0f);
	FRotator rotation(0, 0, 0);
	FActorSpawnParameters parameters;

	boss = GetWorld()->SpawnActor<ACMutant>(location, rotation, parameters);*/

	Print("Spwaning boss");
}

void AU05_CppGameModeBase::DyingCastleGuard()
{
	castleGuardCount--;

	if (castleGuardCount == 0)
		SpawnBoss();
}
