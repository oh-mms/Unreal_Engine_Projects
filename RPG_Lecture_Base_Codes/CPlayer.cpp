// Fill out your copyright notice in the Description page of Project Settings.

#include "CPlayer.h"
#include "CCastleGurad.h"
#include "CLog.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Classes/Materials/Material.h"
#include "Classes/Materials/MaterialInstanceDynamic.h"

// Sets default values
ACPlayer::ACPlayer()
	:	Hp(100),
		Atk(30),
		bAttack(false),
		bNextCombo(false),
		ComboCount(0),
		SkillRange(300)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//////////////////////////////////////////////////////////////////////////////

	static ConstructorHelpers::FObjectFinder<UAnimMontage> attack
	(
		TEXT("/Game/Player/Animations/MonAttack")
	);
	AttackMontage[0] = attack.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> attack2
	(
		TEXT("/Game/Player/Animations/MonAttack2")
	);
	AttackMontage[1] = attack2.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> attack3
	(
		TEXT("/Game/Player/Animations/MonAttack3")
	);
	AttackMontage[2] = attack3.Object;


	static ConstructorHelpers::FObjectFinder<UAnimMontage> jump
	(
		TEXT("/Game/Player/Animations/MonJump")
	);
	JumpMontage = jump.Object;


	static ConstructorHelpers::FObjectFinder<UMaterial> bodyMaterial
	(
		TEXT("/Game/Player/Materials/M_Water_Ocean1")
	);

	if (bodyMaterial.Succeeded() == true)
		BodyMaterial = bodyMaterial.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> swordMaterial
	(
		TEXT("/Game/Player/Materials/Sword_SG1")
	);

	if (swordMaterial.Succeeded() == true)
		SwordMaterial = swordMaterial.Object;
}

// Called when the game starts or when spawned
void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	BodyMaterialInstance = UMaterialInstanceDynamic::Create(BodyMaterial, this);
	GetMesh()->SetMaterial(0, BodyMaterialInstance);

	SwordMaterialInstance = UMaterialInstanceDynamic::Create(SwordMaterial, this);
	GetMesh()->SetMaterial(1, SwordMaterialInstance);

	TSet<UActorComponent *> components = this->GetComponents();
	for (UActorComponent* component : components)
	{
		FString name = component->GetName();
		if (name == "Capsule")
			WeaponCapsule = Cast<UCapsuleComponent>(component);

		/*UE_LOG(LogTemp, Warning, TEXT("Name : %s"), *name);

		Print(component->GetName());*/
	}

	WeaponCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACPlayer::OnBeginOverlap);
	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &ACPlayer::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACPlayer::MoveRight);

	InputComponent->BindAction("Attack", IE_Pressed, this, &ACPlayer::Attacking);
	InputComponent->BindAction("Skill", IE_Pressed, this, &ACPlayer::SkillDamage);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACPlayer::StartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACPlayer::EndJump);
}

void ACPlayer::MoveForward(float axis)
{
	if (bAttack == true)
		return;

	FVector vec = FVector(1, 0, 0);
	AddMovementInput(vec, axis);
}

void ACPlayer::MoveRight(float axis)
{
	if (bAttack == true)
		return;

	FVector vec = FVector(0, 1, 0);
	AddMovementInput(vec, axis);
}

void ACPlayer::Attacking()
{
	if (bAttack == true)
	{
		bNextCombo = true;

		return;
	}

	bAttack = true;

	if (ComboCount < 2)
		PlayAnimMontage(AttackMontage[ComboCount]);
	else
		PlayAnimMontage(AttackMontage[ComboCount], 1, FName("Start"));
}

void ACPlayer::OnEndAttack()
{
	bAttack = false;

	if (bNextCombo == true)
	{
		Attacking();

		bNextCombo = false;
	}
	else
		ComboCount = 0;
}

void ACPlayer::OnCheckCombo()
{
	if (bNextCombo == true)
		ComboCount++;
	/* 임의로 넣어봄. 2타 액션이 2번 연속으로 나감을 방지.
	else
		ComboCount = 0;
	*/
}

void ACPlayer::OnFinishCombo()
{
	bAttack = false;
	bNextCombo = false;

	ComboCount = 0;
}

void ACPlayer::OnRangeAttack()
{
	FVector location = GetActorLocation();
	FVector temp = FVector(location.X, location.Y, 0.0f);

	for (TActorIterator<ACCastleGurad> iter(GetWorld()); iter; ++iter)
	{
		ACCastleGurad* monster = *iter;

		FVector location2 = monster->GetActorLocation();
		FVector temp2 = FVector(location2.X, location2.Y, 0.0f);

		float distance = FVector::Distance(temp, temp2);
		if (distance < SkillRange)
			monster->Damage(Atk);
	}
}

void ACPlayer::OnStartCollision()
{
	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACPlayer::OnEndCollision()
{
	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACPlayer::OnBeginOverlap_Implementation(UPrimitiveComponent * OverComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this)
		return;

	FString str = "Damaged ";
	str.Append(FString::SanitizeFloat(Atk));

	FOutputDeviceNull ar;
	OtherActor->CallFunctionByNameWithArguments(*str, ar, NULL, true);

	ACCastleGurad* temp = Cast<ACCastleGurad>(OtherActor);
	if (temp != NULL)
		temp->Damage(Atk);
}

void ACPlayer::Damage(float damage)
{
	Hp -= damage;

	BodyMaterialInstance->SetVectorParameterValue("_Color", FLinearColor(1, 0, 0, 1));
	SwordMaterialInstance->SetVectorParameterValue("_Color", FLinearColor(1, 0, 0, 1));

	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &ACPlayer::EndDamage, 0.2f);
}

void ACPlayer::EndDamage()
{
	BodyMaterialInstance->SetVectorParameterValue("_Color", FLinearColor(1, 1, 1, 1));
	SwordMaterialInstance->SetVectorParameterValue("_Color", FLinearColor(1, 1, 1, 1));
}

void ACPlayer::StartJump()
{
	bool bFalling = GetCharacterMovement()->IsFalling();
	if (bFalling == false && bAttack == false)
	{
		PlayAnimMontage(JumpMontage);
		Jump();
	}
}

void ACPlayer::EndJump()
{
	StopJumping();
}

void ACPlayer::SkillDamage()
{
	FVector location = GetActorLocation();
	FVector temp = FVector(location.X, location.Y, 0.0f);

	for (TActorIterator<ACCastleGurad> iter(GetWorld()); iter; ++iter)
	{
		ACCastleGurad* monster = *iter;

		FString str = "SkillDamage ";
		str.Append(FString::SanitizeFloat(Atk));

		FOutputDeviceNull ar;
		monster->CallFunctionByNameWithArguments(*str, ar, NULL, true);
	}
}
