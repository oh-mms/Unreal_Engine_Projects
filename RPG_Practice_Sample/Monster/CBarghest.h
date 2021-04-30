// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBarghest.generated.h"

enum class ACBarghestState : uint8
{
	Idle = 0, Follow, Attack, Death,
};

UCLASS()
class SINGLERPG_API ACBarghest : public ACharacter
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

	//UFUNCTION(BlueprintNativeEvent)
	//	void OnBeginOverlap
	//	(
	//		UPrimitiveComponent* OverComp,
	//		AActor* OtherActor,
	//		UPrimitiveComponent* OtherComp,
	//		int32 OtherBodyIndex,
	//		bool bFromSweep,
	//		const FHitResult& SweepResult
	//	);

	//void OnBeginOverlap_Implementation
	//(
	//	UPrimitiveComponent* OverComp,
	//	AActor* OtherActor,
	//	UPrimitiveComponent* OtherComp,
	//	int32 OtherBodyIndex,
	//	bool bFromSweep,
	//	const FHitResult& SweepResult
	//);

public:
	// Sets default values for this character's properties
	ACBarghest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EvenInstigator, AActor* DamageCauser) override;

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
	ACBarghestState State;

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
