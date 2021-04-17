// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CCastleGurad.generated.h"

enum class CastleGuardState : uint8
{
	Idle = 0, Follow, Attack, Death,
};

UCLASS()
class U05_CPP_API ACCastleGurad : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Hp;

	UPROPERTY(EditAnywhere)
		float Distance;

	UPROPERTY(EditAnywhere)
		float Attack;

	UPROPERTY(EditAnywhere)
		float AttackDistance;

	UPROPERTY(EditAnywhere)
		float ReAttackDistance;

	UPROPERTY(EditAnywhere)
		float AttackDelay;

	UFUNCTION(BlueprintCallable)
		void OnCollision();

	UFUNCTION(BlueprintCallable)
		void OffCollision();

	UFUNCTION(BlueprintCallable)
		void OnEndAttack();

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

public:
	// Sets default values for this character's properties
	ACCastleGurad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintCallable)
		void Damage(float damage);
	void EndDamage();

private:
	void Idling();
	void Following();
	void Attacking();
	void Dying();
	void EndDying();

	void SetDistanceAndDirection();
	void OnEndAttackComplete();

private:
	bool bAttack;

	bool bDeath;
	CastleGuardState State;

	class UAnimMontage* DeathMontage;
	class UAnimMontage* AttackMontage;
	class UAnimMontage* DamageMontage;

	class UMaterial* BodyMaterial;
	class UMaterialInstanceDynamic* BodyMaterialInstance;

	class UCapsuleComponent* MainCapsule;
	class UCapsuleComponent* WeaponCapsule;

	float DistanceToPlayer;
	FVector DirectionToPlayer;
};