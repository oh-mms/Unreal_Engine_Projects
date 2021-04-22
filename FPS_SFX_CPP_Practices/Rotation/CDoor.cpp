// Fill out your copyright notice in the Description page of Project Settings.

#include "CDoor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "U14_CppCharacter.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ACDoor::ACDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	Box->InitBoxExtent(FVector(100, 100, 100));
	RootComponent = Box;

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACDoor::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACDoor::OnOverlapEnd);


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
}

// Called when the game starts or when spawned
void ACDoor::BeginPlay()
{
	Super::BeginPlay();
	
	DrawDebugBox(GetWorld(), GetActorLocation(), Box->GetScaledBoxExtent()
		, FQuat(GetActorRotation()), FColor::Red, true, FLT_MAX);
}

// Called every frame
void ACDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotation = Door->RelativeRotation;
	if (bOpen == true)
	{
		FQuat q1 = FQuat(FRotator(0, RotationAngle, 0));
		FQuat q2 = FMath::Lerp(FQuat(Rotation), q1, 0.01f); // Rotation���� q1���� 0.01f��ŭ õõ�� ����

		Door->SetRelativeRotation(q2);
	}
	else
	{
		FQuat q1 = FQuat(FRotator(0, 0, 0));
		FQuat q2 = FMath::Lerp(FQuat(Rotation), q1, 0.01f); // Rotation���� q1���� 0.01f��ŭ õõ�� ����

		Door->SetRelativeRotation(q2);
	}
}

void ACDoor::OnOverlapBegin(UPrimitiveComponent * OverComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	APawn* temp = GetWorld()->GetFirstPlayerController()->GetPawn();
	AActor* actor = Cast<AActor>(temp);

	bool b = true;
	b &= (OtherActor != NULL);
	b &= (OtherActor != this);
	b &= actor != NULL;
	b &= actor == OtherActor;
	
	if (b == false)
		return;


	FVector location = actor->GetActorLocation();
	FVector direction = GetActorLocation() - location;
	direction = UKismetMathLibrary::LessLess_VectorRotator(direction, GetActorRotation()); // A�� -B �������� ȸ����Ŵ. (A�� B�� ���������� ����)

	if (direction.X > 0.0f)
		RotationAngle = 90.0f;
	else
		RotationAngle = -90.0f;

	bOpen = true;
}

void ACDoor::OnOverlapEnd(UPrimitiveComponent * OverComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	APawn* temp = GetWorld()->GetFirstPlayerController()->GetPawn();
	AActor* actor = Cast<AActor>(temp);

	bool b = true;
	b &= (OtherActor != NULL);
	b &= (OtherActor != this);
	b &= actor != NULL;
	b &= actor == OtherActor;

	if (b == false)
		return;

	bOpen = false;
}

