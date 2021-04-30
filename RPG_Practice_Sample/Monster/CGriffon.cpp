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
	
	// 플레이어를 찾지 못 했을 경우를 대비해 마지막으로 플레이어를 본 시간과 플래그 활용.
	LastSeenTime = GetWorld()->GetTimeSeconds();
	bSensedTarget = true;

	// 조건이 만족하면 Blackboard의 타깃 에너미로 지정된 오브젝트 타입값을 감지된 Pawn과 연결.
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
	// 공격중이지 않을 때 맞는 동작을 구현하도록 변수 하나를 선언.
	// 공격 몽타주가 play 되지 않으면 0, 실행되면 1.
	float bMeleeDuring = GetMesh()->AnimScriptInstance->Montage_GetPlayRate(MeleeAnimMontage);

	if (bMeleeDuring == 0)
	{
		PlayAnimMontage(TakeHitAnimMontage);

		if (DamageTaken > 0.0f)
		{
			// 대미지로 인한 운동량 적용.
			ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
		}
	}
}

void ACGriffon::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	// 바인딩 된 함수를 해제함. (아마 DeltaTime 관련 함수들인 듯)
	GetWorldTimerManager().ClearAllTimersForObject(this);

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore); // 충돌체에 반응하는 채널들을 모두 무시상태로 지정.
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
	PawnSensingComp->SetPeripheralVisionAngle(80.0f); // 시야각
	PawnSensingComp->SightRadius = 800; // 시야탐지범위 8미터


	MeleeCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCollision"));
	MeleeCollisionComp->SetRelativeLocation(FVector(45, 0, 25));
	MeleeCollisionComp->InitCapsuleSize(30.0f, 70.0f);

	// 모든 채널타입이 무시될 수 있도록 무시하게 만들고, Player만 탐지하도록 다시 설정.
	// DefaultEngine.ini 중간쯤을 살펴보면, 이전에 미리 프로젝트 세팅에서 설정한 채널이
	// DefaultChannelResponses ... Player 부터 설정돼있는데, Player가 1번임.
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
	//// PrimaryActorTick.bCanEverTick = false; 도 가능

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

