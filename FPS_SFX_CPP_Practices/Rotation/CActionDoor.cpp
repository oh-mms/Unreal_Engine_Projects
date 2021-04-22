// Fill out your copyright notice in the Description page of Project Settings.

#include "CActionDoor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "U14_CppCharacter.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ACActionDoor::ACActionDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CheckArea = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	CheckArea->InitBoxExtent(FVector(150, 100, 100));
	CheckArea->SetCollisionProfileName("Trigger");
	RootComponent = CheckArea;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> door
	(
		TEXT("/Game/StarterContent/Props/SM_Door.SM_Door")
	);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(FName("Door"));
	Door->SetupAttachment(RootComponent);

	if (door.Succeeded())
	{
		Door->SetStaticMesh(door.Object);
		Door->SetRelativeLocation(FVector(0, 50, -100)); // 부모 대비
		Door->SetWorldScale3D(FVector(1.0f)); // 월드 대비
	}

	bClosed = true;

	bOpening = false;
	bClosed = false;

	DotProduct = AddRotation = MaxDegree = 0;
	PosNegative = CurrentRotaiton = 0;
}

// Called when the game starts or when spawned
void ACActionDoor::BeginPlay()
{
	Super::BeginPlay();
	
	DrawDebugBox(GetWorld(), GetActorLocation(), CheckArea->GetScaledBoxExtent()
		, FQuat(GetActorRotation()), FColor::Red, true, FLT_MAX);
}

// Called every frame
void ACActionDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOpening == true)
		OpenDoor(DeltaTime);

	if (bClosed == true)
		CloseDoor(DeltaTime);
}

void ACActionDoor::Toggle(FVector Forward)
{
	DotProduct = FVector::DotProduct(CheckArea->GetForwardVector(), Forward);
	PosNegative = FMath::Sign(DotProduct); // 음수인지 양수인지 판별
	MaxDegree = PosNegative * 90.0f;

	if (bClosed == true)
	{
		bClosed = false;

		bOpening = true;
		bClosing = false;
	}
	else
	{
		bClosed = true;
		
		bOpening = false;
		bClosing = true;
	}
}

void ACActionDoor::OpenDoor(float DeltaTime)
{
	CurrentRotaiton = Door->RelativeRotation.Yaw; // 수직축
	AddRotation = PosNegative * DeltaTime * 80.0f;

	// Lerp보다 이게 더 정확한 방법. Lerp는 실제로 다 열린 것 처럼 보여도, 실제로는 계속 움직이도 있음. (보간이라)
	// 게임에서는 같다라는 기호를 절대 안 쓰기 때문에 근사치로 처리. 근사치까지 도달했다면.
	if (FMath::IsNearlyEqual(CurrentRotaiton, MaxDegree, 1.5f))
	{
		bClosing = false;
		bOpening = false;
	}
	else if (bOpening == true)
	{
		FRotator rot = FRotator(0, AddRotation, 0);

		Door->AddRelativeRotation(rot);
	}
}

void ACActionDoor::CloseDoor(float DeltaTime)
{
	CurrentRotaiton = Door->RelativeRotation.Yaw;

	// 현재 문이 열린 상태(Yaw 축이 큰 상태)면 0으로 보내서 닫힌 상태로 만들어준다.
	if (CurrentRotaiton > 0.0f)
		AddRotation = -DeltaTime * 80.0f;
	else
		AddRotation = DeltaTime * 80.0f;

	if (FMath::IsNearlyZero(CurrentRotaiton, 1.5f)) // 세번째 값은 마찬가지로 얼마나 근접했는가?
	{
		bClosing = false;
		bOpening = false;
	}
	else if (bClosing == true)
	{
		FRotator rot = FRotator(0, AddRotation, 0);

		Door->AddRelativeRotation(rot);
	}
}

