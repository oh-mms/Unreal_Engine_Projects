// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "U05_CppGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class U05_CPP_API AU05_CppGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AU05_CppGameModeBase();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float delta) override;

public:
	UPROPERTY(BlueprintReadOnly)
		float PlayerHpRatio;

	// юс╫ц
	//class ACMutant* boss;

	UFUNCTION(BlueprintCallable)
		void SpawnBoss();

public:
	void DyingCastleGuard();

private:
	class ACPlayer* player;

	UINT castleGuardCount;
};
