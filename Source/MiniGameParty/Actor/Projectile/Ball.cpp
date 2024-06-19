#include "Actor/Projectile/Ball.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Actor/BallMachine.h"

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
	Projectile->MaxSpeed = 1000.f;
	Projectile->InitialSpeed = 1000.f;
	InitialLifeSpan = 30.f;

	RollDirection = FRotator(-10, 0, 0);
	OnActorHit.AddDynamic(this, &ThisClass::OnActorHitFunction);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();

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
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetSimulatePhysics(true);
	Sphere->AddImpulse(AddForce * (Projectile->MaxSpeed / 1000));
	Sphere->SetNotifyRigidBodyCollision(true);

	bHitted = true;
	HitBall = true;
	HitLocation = GetActorLocation();
}

void ABall::SetSpeed()
{
	int32 Random = FMath::RandRange(1, 3);
	float Speed = 1000 * Random;

	if (Projectile)
	{
		Projectile->InitialSpeed = Speed;
		Projectile->MaxSpeed = Speed;
	}
}

void ABall::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(Hit.Component))
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		Sphere->SetSimulatePhysics(true);
		bHitted = true;
	}
}

void ABall::OnActorHitFunction(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(Hit.Component))
	{
		if (HitBall)
		{
			HitBall = false;
			float HitDistance = (HitLocation - GetActorLocation()).Size();
			HitDistance = HitDistance / 100;
			if (ABallMachine* BallMachine = Cast<ABallMachine>(GetOwner()))
			{
				BallMachine->SetDistnace(HitDistance);
			}
		}
	}
}

