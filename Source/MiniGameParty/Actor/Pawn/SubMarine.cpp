#include "Actor/Pawn/SubMarine.h"
#include "Components/SphereComponent.h"

ASubMarine::ASubMarine()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;

	SubMarineMovementComponent = CreateDefaultSubobject<USubMarineMovementComponent>(TEXT("SubMarineMovementComponent"));
	SubMarineMovementComponent->UpdatedComponent = RootComponent;
	SubMarineMovementComponent->SetPawnOwner(this);
}

void ASubMarine::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASubMarine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASubMarine::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASubMarine::OnEngine()
{

}

