// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Monster/CBarghest.h"
#include "../Monster/CGriffon.h"

// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordMesh"));
	RootComponent = SwordMesh;

	// ConstructorHelpers로 객체를 불러올 때, static을 사용해서 공유할 수 있도록 만들어주는 것이 좋다. (인스턴스마다 불러올 필요 없음)
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> swordMesh(TEXT("SkeletalMesh'/Game/LordVictor/Mesh/SK_Kantana.SK_Kantana_SM_Kantana'"));
	SwordMesh->SetSkeletalMesh(swordMesh.Object);

	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollision->InitCapsuleSize(30.0f, 50.0f);
	SwordCollision->AttachToComponent(SwordMesh, FAttachmentTransformRules::KeepRelativeTransform, "CapsuleSocket");
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASword::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// GameStaticObject 클래스에 정의돼있는 ApplyDamage를 가져와 OtherActor가 존재한다면 데미지를 발생.
	if (OtherActor->IsA(AActor::StaticClass()))
	{
		if (MyPawn->bAttack == true)
		{
			UGameplayStatics::ApplyDamage(OtherActor, 10.0f, NULL, this, UDamageType::StaticClass());
		}

		////ACBarghest* Enemy = Cast<ACBarghest>(OtherActor);
		//ACGriffon* Enemy = Cast<ACGriffon>(OtherActor);
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::FromInt(Enemy->Hp));
	}
}

void ASword::SetOwingPawn(ACPlayerBase* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		MyPawn = NewOwner;
	}
}

