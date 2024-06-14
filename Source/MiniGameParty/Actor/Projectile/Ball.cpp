#include "Actor/Projectile/Ball.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	{
		Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
		Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
		Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
	}
	SetRootComponent(Sphere);
	Ball->SetupAttachment(RootComponent);

	Projectile->InitialSpeed = 1000.f;
	Projectile->MaxSpeed = 1000.f;
	Projectile->ProjectileGravityScale = 0.f;
	InitialLifeSpan = 30.f;

	RollDirection = FRotator(-10, 0, 0);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Ball->AddRelativeRotation(RollDirection.Quaternion());
}

void ABall::HitBat(FVector AddForce)
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Sphere->SetSimulatePhysics(true);
	Sphere->AddImpulse(AddForce);
}

