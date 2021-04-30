// Fill out your copyright notice in the Description page of Project Settings.


#include "CBarghest.h"
#include "../Player/CPlayerBase.h"
#include "TimerManager.h"
#include "Animation/AnimMontage.h"
#include "../SingleRPGGameModeBase.h"
//#include "Classes/Materials/Material.h"
//#include "Classes/Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

void ACBarghest::OnCollision()
{
}

void ACBarghest::OffCollision()
{
}

void ACBarghest::OnEndAttack()
{
}

// Sets default values
ACBarghest::ACBarghest()
	:	State(ACBarghestState::Idle),
		Hp(100),
		Attack(10),
		Distance(300),
		AttackDistance(150),
		ReAttackDistance(180),
		AttackDelay(2)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> deathMontage
	(
		TEXT("AnimMontage'/Game/QuadrapedCreatures/Barghest/Animations/MonDeath.MonDeath'")
	);

	if (deathMontage.Succeeded() == true)
		DeathMontage = deathMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> attackMontage
	(
		TEXT("AnimMontage'/Game/QuadrapedCreatures/Barghest/Animations/MonAttack.MonAttack'")
	);

	if (attackMontage.Succeeded() == true)
		AttackMontage = attackMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> damagedMontage
	(
		TEXT("AnimMontage'/Game/QuadrapedCreatures/Barghest/Animations/MonDamaged.MonDamaged'")
	);

	if (damagedMontage.Succeeded() == true)
		DamagedMontage = damagedMontage.Object;


	//static ConstructorHelpers::FObjectFinder<UMaterial> bodyMaterial
	//(
	//	TEXT("/Game/CastleGuard/Mesh/CastleGaurdMaterial")
	//);

	//if (bodyMaterial.Succeeded() == true)
	//	BodyMaterial = bodyMaterial.Object;
}

// Called when the game starts or when spawned
void ACBarghest::BeginPlay()
{
	Super::BeginPlay();
	
	//BodyMaterialInstance = UMaterialInstanceDynamic::Create(BodyMaterial, this);
	//GetMesh()->SetMaterial(0, BodyMaterialInstance);

	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components)
	{
		if (component->GetName() == "Capsule")
			WeaponCapsule = Cast<UCapsuleComponent>(component);

		if (component->GetName() == "CollisionCylinder")
			MainCapsule = Cast<UCapsuleComponent>(component);

		//// UE_LOG(LogTemp, Warning, TEXT("%s"), *component->GetName());
	}
//	WeaponCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACBarghest::OnBeginOverlap);
	//WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OffCollision();
}

// Called every frame
void ACBarghest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDeath == true)
		return;
	// PrimaryActorTick.bCanEverTick = false; µµ °¡´É

	SetDistanceAndDirection();

	switch (State)
	{
	case ACBarghestState::Idle:
		Idling();
		break;
	case ACBarghestState::Follow:
		Following();
		break;
	case ACBarghestState::Attack:
		Attacking();
		break;
	case ACBarghestState::Death:
		Dying();
		break;
	}
}

// Called to bind functionality to input
void ACBarghest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACBarghest::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EvenInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EvenInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		Hp -= ActualDamage;
		PlayAnimMontage(DamagedMontage);
	}

	return ActualDamage;
}

void ACBarghest::Damage(float damage)
{
}

void ACBarghest::EndDamage()
{
}

void ACBarghest::Idling()
{
	if (DistanceToPlayer <= Distance)
		State = ACBarghestState::Follow;
}

void ACBarghest::Following()
{
	if (DistanceToPlayer <= AttackDistance)
		State = ACBarghestState::Attack;

/*	if (DistanceToPlayer <= Distance)
		*/AddMovementInput(DirectionToPlayer);
}

void ACBarghest::Attacking()
{
}

void ACBarghest::Dying()
{
}

void ACBarghest::EndDying()
{
}

void ACBarghest::SetDistanceAndDirection()
{
}

void ACBarghest::OnEndAttackComplete()
{
}

