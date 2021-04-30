// Fill out your copyright notice in the Description page of Project Settings.


#include "CGriffon.h"
#include "../Player/CPlayerBase.h"
#include "TimerManager.h"
#include "Animation/AnimMontage.h"
#include "../SingleRPGGameModeBase.h"
//#include "Classes/Materials/Material.h"
//#include "Classes/Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "GriffonAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

void ACGriffon::OnCollision()
{
}

void ACGriffon::OffCollision()
{
}

void ACGriffon::OnEndAttack()
{
}

void ACGriffon::OnSeePlayer(APawn* Pawn)
{
	AGriffonAIController* AIController = Cast<AGriffonAIController>(GetController());
	ACPlayerBase* SensedPawn = Cast<ACPlayerBase>(Pawn);
	
	// �÷��̾ ã�� �� ���� ��츦 ����� ���������� �÷��̾ �� �ð��� �÷��� Ȱ��.
	LastSeenTime = GetWorld()->GetTimeSeconds();
	bSensedTarget = true;

	// ������ �����ϸ� Blackboard�� Ÿ�� ���ʹ̷� ������ ������Ʈ Ÿ�԰��� ������ Pawn�� ����.
	if (AIController != nullptr && SensedPawn->Hp > 0)
	{
		AIController->SetEnemy(SensedPawn);
	}
}

void ACGriffon::OnMeleeCompBeginOverlap(UPrimitiveComponent* OverComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ExecuteMeleeDamage(OtherActor);
}

void ACGriffon::ExecuteMeleeDamage(AActor* HitActor)
{
	ACPlayerBase* PlayerPawn = Cast<ACPlayerBase>(HitActor);

	if (HitActor && HitActor != this && PlayerPawn->Hp > 0)
	{
		if (PlayerPawn && Hp > 0)
		{
			GLog->Log("Griffon->Player overlap");
			PlayAnimMontage(MeleeAnimMontage);

			FPointDamageEvent DmgEvent;

			HitActor->TakeDamage(50, DmgEvent, GetController(), this);
		}
	}
}

void ACGriffon::OnHit(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	// ���������� ���� �� �´� ������ �����ϵ��� ���� �ϳ��� ����.
	// ���� ��Ÿ�ְ� play ���� ������ 0, ����Ǹ� 1.
	float bMeleeDuring = GetMesh()->AnimScriptInstance->Montage_GetPlayRate(MeleeAnimMontage);

	if (bMeleeDuring == 0)
	{
		PlayAnimMontage(TakeHitAnimMontage);

		if (DamageTaken > 0.0f)
		{
			// ������� ���� ��� ����.
			ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
		}
	}
}

void ACGriffon::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	// ���ε� �� �Լ��� ������. (�Ƹ� DeltaTime ���� �Լ����� ��)
	GetWorldTimerManager().ClearAllTimersForObject(this);

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore); // �浹ü�� �����ϴ� ä�ε��� ��� ���û��·� ����.
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	if (Controller != NULL)
	{
		Controller->UnPossess();
	}

	PlayAnimMontage(TakeHitAnimMontage);

	SetRagdollPhysics();
}

void ACGriffon::SetRagdollPhysics()
{
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;

	SetLifeSpan(3.5f);
}

// Sets default values
ACGriffon::ACGriffon()
	: Hp(100),
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

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(80.0f); // �þ߰�
	PawnSensingComp->SightRadius = 800; // �þ�Ž������ 8����


	MeleeCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCollision"));
	MeleeCollisionComp->SetRelativeLocation(FVector(45, 0, 25));
	MeleeCollisionComp->InitCapsuleSize(30.0f, 70.0f);

	// ��� ä��Ÿ���� ���õ� �� �ֵ��� �����ϰ� �����, Player�� Ž���ϵ��� �ٽ� ����.
	// DefaultEngine.ini �߰����� ���캸��, ������ �̸� ������Ʈ ���ÿ��� ������ ä����
	// DefaultChannelResponses ... Player ���� �������ִµ�, Player�� 1����.
	MeleeCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeCollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	MeleeCollisionComp->SetupAttachment(GetCapsuleComponent());

	//static ConstructorHelpers::FObjectFinder<UMaterial> bodyMaterial
	//(
	//	TEXT("/Game/CastleGuard/Mesh/CastleGaurdMaterial")
	//);

	//if (bodyMaterial.Succeeded() == true)
	//	BodyMaterial = bodyMaterial.Object;

}

// Called when the game starts or when spawned
void ACGriffon::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComp != nullptr)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &ACGriffon::OnSeePlayer);
	}
	
	if (MeleeCollisionComp)
	{
		MeleeCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACGriffon::OnMeleeCompBeginOverlap);
	}

	//BodyMaterialInstance = UMaterialInstanceDynamic::Create(BodyMaterial, this);
	//GetMesh()->SetMaterial(0, BodyMaterialInstance);

	//TSet<UActorComponent*> components = GetComponents();
	//for (UActorComponent* component : components)
	//{
	//	if (component->GetName() == "Capsule")
	//		WeaponCapsule = Cast<UCapsuleComponent>(component);

	//	if (component->GetName() == "CollisionCylinder")
	//		MainCapsule = Cast<UCapsuleComponent>(component);

	//	//// UE_LOG(LogTemp, Warning, TEXT("%s"), *component->GetName());
	//}
	//	WeaponCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACBarghest::OnBeginOverlap);
		//WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OffCollision();
}

// Called every frame
void ACGriffon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSensedTarget != false && (GetWorld()->TimeSeconds - LastSeenTime) > 2.5f)
	{
		AGriffonAIController* AIController = Cast<AGriffonAIController>(GetController());
		if (AIController)
		{
			bSensedTarget = false;
			AIController->SetEnemy(nullptr);
		}
	}

	//if (bDeath == true)
	//	return;
	//// PrimaryActorTick.bCanEverTick = false; �� ����

	//SetDistanceAndDirection();

	//switch (State)
	//{
	//case ACBarghestState::Idle:
	//	Idling();
	//	break;
	//case ACBarghestState::Follow:
	//	Following();
	//	break;
	//case ACBarghestState::Attack:
	//	Attacking();
	//	break;
	//case ACBarghestState::Death:
	//	Dying();
	//	break;
	//}
}

// Called to bind functionality to input
void ACGriffon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACGriffon::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		Hp -= ActualDamage;

		if (Hp <= 0)
		{
			Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
		}
		else
		{
			OnHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		}
		GLog->Log("TakenDamage Griffon");
	}

	if (Hp <= 0.0f)
	{
		return 0.0f;
	}

	return ActualDamage;
}

void ACGriffon::Damage(float damage)
{
}

void ACGriffon::EndDamage()
{
}

void ACGriffon::Idling()
{
}

void ACGriffon::Following()
{

}

void ACGriffon::Attacking()
{
}

void ACGriffon::Dying()
{
}

void ACGriffon::EndDying()
{
}

void ACGriffon::SetDistanceAndDirection()
{
}

void ACGriffon::OnEndAttackComplete()
{
}

