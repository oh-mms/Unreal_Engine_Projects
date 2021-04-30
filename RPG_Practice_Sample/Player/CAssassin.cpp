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

	// ConstructorHelper 는 생성자에만 사용 가능하다.
	// 런타임에 에셋 로딩을 원하면 staticloadobject 를 사용하면 됨.
	// 	초기 시점에 사용할 에셋이 정해져있다면 생성자에서 정의하여 검증하는 것이 훨씬 안전.
	// 언리얼에서 new로 만든 클래스는 지울 수가 없게 되어있음.
	// 따라서 NewObject를 통해서 동적할당을 해야 함.
	UMyCameraShake* shake = NewObject<UMyCameraShake>();

	// ShakeClass는 객체를 갖는 것이 아닌, 자료형을 받는 것.
	ShakeClass = shake->GetClass();


	//static ConstructorHelpers::FObjectFinder<UAnimMontage> jumpLoop
	//(
	//	TEXT("/Game/LordVictor/Animations/Frank/MonJumpLoop")
	//);
	//JumpLoopMontage = jumpLoop.Object;

	// DefaultGame.ini에 1000.f이라 지정해두었으므로, 이는 게임 세팅으로 오버라이드 될 것임.
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

	// 마우스 회전, 감도조절이 필요하다면 프로젝트 세팅의 scale을 조절 or 감도 조절 함수를 만든다.
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
	// 내장 jump 판단 변수. true가 되면 내장 Jump가 호출되어 위로 힘이 들어감.
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
