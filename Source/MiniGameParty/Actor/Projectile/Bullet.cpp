#include "Actor/Projectile/Bullet.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	{
		Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
		SetRootComponent(Scene);
		Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
		ThrowParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Throw"));
		Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	}

	Sphere->SetupAttachment(Scene);
	ThrowParticle->SetupAttachment(Sphere);

	Projectile->InitialSpeed = 3000.f;
	Projectile->MaxSpeed = 3000.f;
	Projectile->ProjectileGravityScale = 0.f;
	Projectile->bSweepCollision = true;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	InitialLifeSpan = 10.f;
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

