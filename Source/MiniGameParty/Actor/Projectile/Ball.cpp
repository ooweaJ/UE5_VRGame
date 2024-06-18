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


	Projectile->ProjectileGravityScale = 0.f;
	InitialLifeSpan = 30.f;

	RollDirection = FRotator(-10, 0, 0);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();

	int32 Random = FMath::RandRange(1, 3);
	Projectile->InitialSpeed = 1000 * Random;
	Projectile->MaxSpeed = 1000 * Random;
	Sphere->OnComponentHit.AddDynamic(this, &ThisClass::OnComponentHit);
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bHitted)
	{
		Ball->AddRelativeRotation(RollDirection.Quaternion());
	}
}

void ABall::HitBat(FVector AddForce)
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Sphere->SetSimulatePhysics(true);
	Sphere->AddImpulse(AddForce * (Projectile->MaxSpeed / 1000));
	bHitted = true;
}

void ABall::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Sphere->SetSimulatePhysics(true);
	bHitted = true;
}

