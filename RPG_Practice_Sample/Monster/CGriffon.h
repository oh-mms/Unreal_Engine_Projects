// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CGriffon.generated.h"

UCLASS()
class SINGLERPG_API ACGriffon : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
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

public:
	// Sets default values for this character's properties
	ACGriffon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

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

public:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BotBehavior;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UAnimMontage* MeleeAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UAnimMontage* TakeHitAnimMontage;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		UCapsuleComponent* MeleeCollisionComp;

protected:
	UFUNCTION()
		void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
		void OnMeleeCompBeginOverlap
		(
			UPrimitiveComponent* OverComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

public:
	float LastSeenTime;
	bool bSensedTarget;

	UFUNCTION(BlueprintCallable, Category = "AI")
		void ExecuteMeleeDamage(AActor* HitActor);

	virtual void OnHit
	(
		float DamageTaken,
		struct FDamageEvent const& DamageEvent,
		class APawn* PawnInstigator,
		class AActor* DamageCauser
	);

	virtual void Die
	(
		float KillingDamage,
		struct FDamageEvent const& DamageEvent,
		AController* Killer,
		AActor* DamageCauser
	);

	void SetRagdollPhysics();

private:
	bool bAttack;

	bool bDeath;

	class UAnimMontage* DeathMontage;
	class UAnimMontage* AttackMontage;
	class UAnimMontage* DamagedMontage;

	class UMaterial* BodyMaterial;
	class UMaterialInstanceDynamic* BodyMaterialInstance;

	class UCapsuleComponent* MainCapsule;
	class UCapsuleComponent* WeaponCapsule;

	float DistanceToPlayer;
	FVector DirectionToPlayer;
};
