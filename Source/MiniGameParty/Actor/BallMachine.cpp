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
}

void ABallMachine::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABallMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

