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
		Door->SetRelativeLocation(FVector(0, 50, -100)); // �θ� ���
		Door->SetWorldScale3D(FVector(1.0f)); // ���� ���
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
	PosNegative = FMath::Sign(DotProduct); // �������� ������� �Ǻ�
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
	CurrentRotaiton = Door->RelativeRotation.Yaw; // ������
	AddRotation = PosNegative * DeltaTime * 80.0f;

	// Lerp���� �̰� �� ��Ȯ�� ���. Lerp�� ������ �� ���� �� ó�� ������, �����δ� ��� �����̵� ����. (�����̶�)
	// ���ӿ����� ���ٶ�� ��ȣ�� ���� �� ���� ������ �ٻ�ġ�� ó��. �ٻ�ġ���� �����ߴٸ�.
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

	// ���� ���� ���� ����(Yaw ���� ū ����)�� 0���� ������ ���� ���·� ������ش�.
	if (CurrentRotaiton > 0.0f)
		AddRotation = -DeltaTime * 80.0f;
	else
		AddRotation = DeltaTime * 80.0f;

	if (FMath::IsNearlyZero(CurrentRotaiton, 1.5f)) // ����° ���� ���������� �󸶳� �����ߴ°�?
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

