// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class U05_CPP_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	class UAnimMontage* AttackMontage[3];
	class UAnimMontage* JumpMontage;

private:
	void MoveForward(float axis);
	void MoveRight(float axis);

public:
	UPROPERTY(EditAnywhere)
		float Hp;

	UPROPERTY(EditAnywhere)
		float Atk;

	UPROPERTY(EditAnywhere)
		float SkillRange;

	UFUNCTION(BlueprintCallable)
		void OnEndAttack();

	UFUNCTION(BlueprintCallable)
		void OnCheckCombo();

	UFUNCTION(BlueprintCallable)
		void OnFinishCombo();

	UFUNCTION(BlueprintCallable)
		void OnRangeAttack();

	UFUNCTION(BlueprintCallable)
		void OnStartCollision();

	UFUNCTION(BlueprintCallable)
		void OnEndCollision();

	UFUNCTION(BlueprintNativeEvent)
		void OnBeginOverlap
		(
			UPrimitiveComponent* OverComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	void OnBeginOverlap_Implementation
	(
		UPrimitiveComponent* OverComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	void Damage(float damage);
	void EndDamage();

	void Attacking();

	void StartJump();
	void EndJump();

	void SkillDamage();

private:
	class UCapsuleComponent* WeaponCapsule;

	class UMaterial* BodyMaterial;
	class UMaterialInstanceDynamic* BodyMaterialInstance;

	class UMaterial* SwordMaterial;
	class UMaterialInstanceDynamic* SwordMaterialInstance;

	bool bAttack;
	bool bNextCombo;
	UINT ComboCount;
};
