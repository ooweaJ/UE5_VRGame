#include "Actor/Projectile/Bullet.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Actor/Pawn/Enemy/Shark.h"
#include "Engine/DamageEvents.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	{
		SetRootComponent(Sphere);
		Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
		ThrowParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Throw"));
		Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	}

	ThrowParticle->SetupAttachment(Sphere);

	Projectile->InitialSpeed = 1000.f;
	Projectile->MaxSpeed = 1000.f;
	Projectile->ProjectileGravityScale = 0.f;
	InitialLifeSpan = 10.f;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentHit.AddDynamic(this, &ThisClass::OnComponentHit);
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::SetVelocity(FVector InputVelocity)
{
	Projectile->Velocity = InputVelocity;
}

void ABullet::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!!ImpactParticle)
	{
		FVector location = Hit.Location;
		FRotator Rotation = Projectile->Velocity.GetSafeNormal().Rotation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticle, location, Rotation);
	}
	AShark*Shark = Cast<AShark>(OtherActor);
	if (Shark)
	{
		FDamageEvent de;
		Shark->TakeDamage(1.f, de, GetOwner()->GetInstigatorController(), GetOwner());
	}

	Destroy();
}

