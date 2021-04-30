// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayerBase.generated.h"

UCLASS()
class SINGLERPG_API ACPlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPlayerBase();

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
	class UAnimMontage* JumpEndMontage;
	//class UAnimMontage* JumpLoopMontage;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void StartJump();
	void EndJump();
	//void JumpLoop();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
		float Hp;

	UPROPERTY(config, BlueprintReadWrite, EditAnywhere, Category = "Health")
		float MaxHp;

	UPROPERTY(EditAnywhere)
		float Atk;

	UPROPERTY(EditAnywhere)
		float SkillRange;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<class ASword> SwordClass;

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

	void Damage(float damage);
	void EndDamage();

	void Attacking();

	void SkillDamage();

protected:
	void SpawnDefaultInventory();

private:
	class UCapsuleComponent* WeaponCapsule;

	class UMaterial* BodyMaterial;
	class UMaterialInstanceDynamic* BodyMaterialInstance;

	class UMaterial* SwordMaterial;
	class UMaterialInstanceDynamic* SwordMaterialInstance;

	UPROPERTY(EditAnywhere)
		bool bNextCombo;

	UPROPERTY(EditAnywhere)
		int32 ComboCount;

public:
	UPROPERTY(EditAnywhere)
		bool bAttack;

public:
	UPROPERTY(EditAnywhere)
		float LerpSpeed;

private:
	FVector Offset;

public:
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "MainCamera"))
		AActor* Camera;

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCameraShake> ShakeClass;
};
