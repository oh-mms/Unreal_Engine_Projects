// Fill out your copyright notice in the Description page of Project Settings.

#include "CCastleGurad.h"
#include "CPlayer.h"
#include "CLog.h"
#include "ConstructorHelpers.h"
#include "TimerManager.h"
#include "Animation/AnimMontage.h"
#include "U05_CppGameModeBase.h"
#include "Classes/Materials/Material.h"
#include "Classes/Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACCastleGurad::ACCastleGurad()
	:	State(CastleGuardState::Idle),
		Hp(100),
		Attack(10),
		Distance(300),
		AttackDistance(150),
		ReAttackDistance(180),
		AttackDelay(2)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//////////////////////////////////////////////////////////////////////////////

	static ConstructorHelpers::FObjectFinder<UAnimMontage> deathMontage
	(
		TEXT("/Game/CastleGuard/Animations/MonDying")
	);

	if (deathMontage.Succeeded() == true)
		DeathMontage = deathMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> attackMontage
	(
		TEXT("/Game/CastleGuard/Animations/MonAttack")
	);

	if (attackMontage.Succeeded() == true)
		AttackMontage = attackMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> damageMontage
	(
		TEXT("/Game/CastleGuard/Animations/MonDamage")
	);

	if (damageMontage.Succeeded() == true)
		DamageMontage = damageMontage.Object;


	static ConstructorHelpers::FObjectFinder<UMaterial> bodyMaterial
	(
		TEXT("/Game/CastleGuard/Mesh/CastleGaurdMaterial")
	);

	if (bodyMaterial.Succeeded() == true)
		BodyMaterial = bodyMaterial.Object;

	//TODO: 비주얼스튜디오 투두 기능써보기
}

// Called when the game starts or when spawned
void ACCastleGurad::BeginPlay()
{
	Super::BeginPlay();

	BodyMaterialInstance = UMaterialInstanceDynamic::Create(BodyMaterial, this);
	GetMesh()->SetMaterial(0, BodyMaterialInstance);

	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components)
	{
		if (component->GetName() == "Capsule")
			WeaponCapsule = Cast<UCapsuleComponent>(component);

		if (component->GetName() == "CollisionCylinder")
			MainCapsule = Cast<UCapsuleComponent>(component);

		//// UE_LOG(LogTemp, Warning, TEXT("%s"), *component->GetName());
	}
	WeaponCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACCastleGurad::OnBeginOverlap);
	//WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OffCollision();
}

// Called every frame
void ACCastleGurad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDeath == true)
		return;
	// PrimaryActorTick.bCanEverTick = false; 도 가능

	SetDistanceAndDirection();

	switch (State)
	{
		case CastleGuardState::Idle:
			Idling();
			break;
		case CastleGuardState::Follow:
			Following();
			break;
		case CastleGuardState::Attack:
			Attacking();
			break;
		case CastleGuardState::Death:
			Dying();
			break;
	}
}

// Called to bind functionality to input
void ACCastleGurad::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACCastleGurad::Damage(float damage)
{
	if (bDeath == true)
		return;

	Hp -= damage;

	float time = PlayAnimMontage(DamageMontage);
	//BodyMaterialInstance->SetVectorParameterValue("_Color", FLinearColor(1, 0, 0, 1));

	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &ACCastleGurad::EndDamage, 0.2f);
}

void ACCastleGurad::EndDamage()
{
	//BodyMaterialInstance->SetVectorParameterValue("_Color", FLinearColor(1, 1, 1, 1));

	OnEndAttack();
	 
	if (Hp <= 0.0f)
		State = CastleGuardState::Death;
}

void ACCastleGurad::Idling()
{
	if (DistanceToPlayer <= Distance)
		State = CastleGuardState::Follow;


}

void ACCastleGurad::Following()
{
	if (DistanceToPlayer <= AttackDistance)
		State = CastleGuardState::Attack;

	if (DistanceToPlayer <= Distance)
		AddMovementInput(DirectionToPlayer);
}

void ACCastleGurad::Attacking()
{
	if (bAttack == true)
		return;

	bAttack = true;
	PlayAnimMontage(AttackMontage);
}

void ACCastleGurad::Dying()
{
	if (bDeath == true)
		return;

	bDeath = true;

	OffCollision();
	MainCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	float time = PlayAnimMontage(DeathMontage);

	// 블루프린트 함수를 콜하는 방법.
	FOutputDeviceNull od;
	CallFunctionByNameWithArguments(TEXT("DestroyHpBar"), od, this, true);

	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &ACCastleGurad::EndDying, time + 3.0f);
}

void ACCastleGurad::EndDying()
{
	AU05_CppGameModeBase* gameMode = Cast<AU05_CppGameModeBase>(GetWorld()->GetAuthGameMode());
	gameMode->DyingCastleGuard();

	Destroy();
}

void ACCastleGurad::SetDistanceAndDirection()
{
	FVector location = GetActorLocation();

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector playerLocation = player->GetActorLocation();

	FVector subract = playerLocation - location;

	FVector xy;
	xy.X = subract.X;
	xy.Y = subract.Y;
	xy.Z = 0.0f;

	xy.ToDirectionAndLength(DirectionToPlayer, DistanceToPlayer);
	DirectionToPlayer.Normalize();

	FRotator rotator = xy.Rotation();
	SetActorRotation(rotator);
}

void ACCastleGurad::OnCollision()
{
	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACCastleGurad::OffCollision()
{
	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACCastleGurad::OnEndAttack()
{
	if (DistanceToPlayer < ReAttackDistance)
	{
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &ACCastleGurad::OnEndAttackComplete, AttackDelay);
	}
	else
		OnEndAttackComplete();
}

void ACCastleGurad::OnEndAttackComplete()
{
	bAttack = false;

	State = CastleGuardState::Follow;
}

void ACCastleGurad::OnBeginOverlap_Implementation(UPrimitiveComponent * OverComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this)
		return;

	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	if (player == NULL)
		return;

	//Print(*OtherActor->GetName());
	//FString name = OtherActor->GetName();
	//Print(*name);

	player->Damage(Attack);
}