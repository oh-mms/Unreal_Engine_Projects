// Fill out your copyright notice in the Description page of Project Settings.


#include "CAssassin.h"
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
ACAssassin::ACAssassin()
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


	//static ConstructorHelpers::FObjectFinder<UAnimMontage> jumpLoop
	//(
	//	TEXT("/Game/LordVictor/Animations/Frank/MonJumpLoop")
	//);
	//JumpLoopMontage = jumpLoop.Object;

	// DefaultGame.ini�� 1000.f�̶� �����صξ����Ƿ�, �̴� ���� �������� �������̵� �� ����.
	MaxHp = 100.0f;

}

// Called when the game starts or when spawned
void ACAssassin::BeginPlay()
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
void ACAssassin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACAssassin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACAssassin::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACAssassin::MoveRight);

	// ���콺 ȸ��, ���������� �ʿ��ϴٸ� ������Ʈ ������ scale�� ���� or ���� ���� �Լ��� �����.
	PlayerInputComponent->BindAxis("Turn", this, &ACAssassin::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACAssassin::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACAssassin::Attacking);
	PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &ACAssassin::SkillDamage);
	PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &ACAssassin::SkillDamage);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACAssassin::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACAssassin::EndJump);
}

void ACAssassin::MoveForward(float Value)
{
	if (bAttack == true)
		return;

	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ACAssassin::MoveRight(float Value)
{
	if (bAttack == true)
		return;

	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ACAssassin::StartJump()
{
	// ���� jump �Ǵ� ����. true�� �Ǹ� ���� Jump�� ȣ��Ǿ� ���� ���� ��.
	bool bFalling = GetCharacterMovement()->IsFalling();
	if (bFalling == false && bAttack == false)
	{
		PlayAnimMontage(JumpMontage);
		Jump();
	}
}

void ACAssassin::EndJump()
{
	bool bFalling = GetCharacterMovement()->IsFalling();
	if (bFalling == false && bAttack == false)
	{
		//PlayAnimMontage(JumpEndMontage);
		StopJumping();
	}
}

void ACAssassin::Damage(float damage)
{
}

void ACAssassin::EndDamage()
{
}

void ACAssassin::Attacking()
{

}

void ACAssassin::SpawnDefaultInventory()
{
	FActorSpawnParameters SpawnInfo;

	ASword* DefaultWeapon = GetWorld()->SpawnActor<ASword>(SwordClass, SpawnInfo);
	if (DefaultWeapon != NULL)
		DefaultWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
}

void ACAssassin::OnEndAttack()
{

}

void ACAssassin::OnCheckCombo()
{

}

void ACAssassin::OnFinishCombo()
{

}

void ACAssassin::SkillDamage()
{
}

void ACAssassin::OnRangeAttack()
{
}

void ACAssassin::OnStartCollision()
{
}

void ACAssassin::OnEndCollision()
{
}
