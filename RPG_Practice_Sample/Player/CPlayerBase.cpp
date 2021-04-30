// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerBase.h"
#include "../CameraShake/MyCameraShake.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Weapon/Sword.h"


// Sets default values
ACPlayerBase::ACPlayerBase()
	: Hp(100),
	Atk(30),
	bAttack(false),
	bNextCombo(false),
	ComboCount(0),
	SkillRange(300),
	LerpSpeed(10)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ConstructorHelper �� �����ڿ��� ��� �����ϴ�.
	// ��Ÿ�ӿ� ���� �ε��� ���ϸ� staticloadobject �� ����ϸ� ��.
	// 	�ʱ� ������ ����� ������ �������ִٸ� �����ڿ��� �����Ͽ� �����ϴ� ���� �ξ� ����.
	// �𸮾󿡼� new�� ���� Ŭ������ ���� ���� ���� �Ǿ�����.
	// ���� NewObject�� ���ؼ� �����Ҵ��� �ؾ� ��.
	UMyCameraShake* shake = NewObject<UMyCameraShake>();

	// ShakeClass�� ��ü�� ���� ���� �ƴ�, �ڷ����� �޴� ��.
	ShakeClass = shake->GetClass();

	static ConstructorHelpers::FObjectFinder<UAnimMontage> attack1
	(
		TEXT("/Game/LordVictor/Animations/Frank/MonAttack1")
	);
	AttackMontage[0] = attack1.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> attack2
	(
		TEXT("/Game/LordVictor/Animations/Frank/MonAttack2")
	);
	AttackMontage[1] = attack2.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> attack3
	(
		TEXT("/Game/LordVictor/Animations/Frank/MonAttack3")
	);
	AttackMontage[2] = attack3.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> jump
	(
		TEXT("/Game/LordVictor/Animations/Frank/MonJump")
	);
	JumpMontage = jump.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> jumpEnd
	(
		TEXT("/Game/LordVictor/Animations/Frank/MonJumpEnd")
	);
	JumpEndMontage = jumpEnd.Object;

	//static ConstructorHelpers::FObjectFinder<UAnimMontage> jumpLoop
	//(
	//	TEXT("/Game/LordVictor/Animations/Frank/MonJumpLoop")
	//);
	//JumpLoopMontage = jumpLoop.Object;

	// DefaultGame.ini�� 1000.f�̶� �����صξ����Ƿ�, �̴� ���� �������� �������̵� �� ����.
	MaxHp = 100.0f;
}

// Called when the game starts or when spawned
void ACPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	if (Camera == NULL)
	{
		for (TActorIterator<ACameraActor> iter(GetWorld()); iter; ++iter)
		{
			if (iter->GetName() == "MainCamera")
				Camera = *iter;
		}
	}

	SpawnDefaultInventory();

	Hp = MaxHp;
}

// Called every frame
void ACPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayerBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayerBase::MoveRight);

	// ���콺 ȸ��, ���������� �ʿ��ϴٸ� ������Ʈ ������ scale�� ���� or ���� ���� �Լ��� �����.
	PlayerInputComponent->BindAxis("Turn", this, &ACPlayerBase::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACPlayerBase::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACPlayerBase::Attacking);
	PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &ACPlayerBase::SkillDamage);
	PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &ACPlayerBase::SkillDamage);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACPlayerBase::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACPlayerBase::EndJump);

}

void ACPlayerBase::MoveForward(float Value)
{
	//if (bAttack == true)
	//	return;

	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ACPlayerBase::MoveRight(float Value)
{
	/*if (bAttack == true)
		return;*/

	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ACPlayerBase::Attacking()
{
	if (Hp <= 0.0f)
		return;

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

void ACPlayerBase::OnEndAttack()
{
	GLog->Log("AttackEnd");
	bAttack = false;

	if (bNextCombo == true)
	{
		Attacking();

		bNextCombo = false;
	}
	else
		ComboCount = 0;
}

void ACPlayerBase::OnCheckCombo()
{
	GLog->Log("CheckCombo");
	if (bNextCombo == true)
		ComboCount++;
	else
		ComboCount = 0; // 2Ÿ �׼��� 2�� �������� ������ ����.
}

void ACPlayerBase::OnFinishCombo()
{
	GLog->Log("FinishCombo");
	bAttack = false;
	bNextCombo = false;

	ComboCount = 0;
}

void ACPlayerBase::OnRangeAttack()
{
	GLog->Log("OnRangeAttack");
}

void ACPlayerBase::OnStartCollision()
{
	GLog->Log("OnStartCollision");
}

void ACPlayerBase::OnEndCollision()
{
	GLog->Log("OnEndCollision");
}

//void ACPlayerBase::OnBeginOverlap(UPrimitiveComponent* OverComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//}
//
//void ACPlayerBase::OnBeginOverlap_Implementation(UPrimitiveComponent* OverComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//}

void ACPlayerBase::Damage(float damage)
{
}

void ACPlayerBase::EndDamage()
{
}

void ACPlayerBase::SkillDamage()
{
}

void ACPlayerBase::SpawnDefaultInventory()
{
	FActorSpawnParameters SpawnInfo;

	ASword* DefaultWeapon = GetWorld()->SpawnActor<ASword>(SwordClass, SpawnInfo);
	if (DefaultWeapon != NULL)
	{
		DefaultWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
		DefaultWeapon->SetOwingPawn(this);
	}

}

void ACPlayerBase::StartJump()
{
	Jump();
	//// ���� jump �Ǵ� ����. true�� �Ǹ� ���� Jump�� ȣ��Ǿ� ���� ���� ��.
	//bool bFalling = GetCharacterMovement()->IsFalling();
	//if (bFalling == false && bAttack == false)
	//{
	//	PlayAnimMontage(JumpMontage);
	//	Jump();
	//}
}

void ACPlayerBase::EndJump()
{
	StopJumping();
	//bool bFalling = GetCharacterMovement()->IsFalling();
	//if (bFalling == false && bAttack == false)
	//{
	//	//PlayAnimMontage(JumpEndMontage);
	//	StopJumping();
	//}
}

//void ACPlayerBase::JumpLoop()
//{
//
//}

