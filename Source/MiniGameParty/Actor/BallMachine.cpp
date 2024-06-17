#include "Actor/BallMachine.h"
#include "Actor/Projectile/Ball.h"


ABallMachine::ABallMachine()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(Mesh);

	{
		ConstructorHelpers::FClassFinder<ABall> Class(TEXT("/Script/Engine.Blueprint'/Game/_Dev/Actor/BP_Ball.BP_Ball_C'"));
		if (Class.Succeeded())
		{
			BallClass = Class.Class;
		}
	}
}

void ABallMachine::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::SpawnBall, 5.0f, true);
}

void ABallMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABallMachine::SpawnBall()
{
	if (BallClass)
	{
		FVector SpawnLocation = SpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = SpawnPoint->GetComponentRotation();

		GetWorld()->SpawnActor<ABall>(BallClass, SpawnLocation, SpawnRotation);
	}
}

