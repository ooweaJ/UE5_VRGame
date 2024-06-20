#include "Actor/Bat.h"
#include "Components/CapsuleComponent.h"
#include "Actor/Projectile/Ball.h"
#include "Engine.h"

ABat::ABat()
{
	PrimaryActorTick.bCanEverTick = true;

	{
		Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
		Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	}
	SetRootComponent(Capsule);
	Mesh->SetupAttachment(RootComponent);

}

void ABat::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentHit.AddDynamic(this, &ThisClass::OnComponentHit);

	PreviousLocation = GetActorLocation();


}

void ABat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	CurrentVelocity = (CurrentLocation - PreviousLocation) / DeltaTime;
	PreviousLocation = CurrentLocation;

	float Mass = 1.0f;
	FVector Acceleration = CurrentVelocity / DeltaTime;
	Force = Mass * Acceleration;
}

void ABat::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(ABall* Ball = Cast<ABall>(OtherActor))
	{
		if (HitSound)
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, Hit.Location);
		Ball->HitBat(Force);
	}
}

