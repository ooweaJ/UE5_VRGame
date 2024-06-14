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

	NumSamples = 5;  // 최근 5개의 프레임을 사용하여 평균 계산
	Mass = 0.85f;  // 배트의 질량 (예: 0.85 kg)
}

void ABat::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentHit.AddDynamic(this, &ThisClass::OnComponentHit);

	FVector InitialPosition = GetActorLocation();
	FVector InitialVelocity = FVector::ZeroVector;

	// 초기 위치와 속도 설정
	for (int32 i = 0; i < NumSamples; ++i)
	{
		PreviousPositions.Add(InitialPosition);
		PreviousVelocities.Add(InitialVelocity);
	}
}

void ABat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 현재 위치 가져오기
	FVector CurrentPosition = GetActorLocation();

	// 속도 계산 (위치의 변화율)
	FVector CurrentVelocity = (CurrentPosition - PreviousPositions.Last()) / DeltaTime;

	// 이전 위치와 속도 업데이트
	PreviousPositions.RemoveAt(0);
	PreviousPositions.Add(CurrentPosition);

	PreviousVelocities.RemoveAt(0);
	PreviousVelocities.Add(CurrentVelocity);

	// 평균 속도 계산
	FVector AverageVelocity = CalculateAverage(PreviousVelocities);

	// 가속도 계산 (속도의 변화율)
	FVector AverageAcceleration = (AverageVelocity - CalculateAverage(PreviousVelocities)) / DeltaTime;

	// 힘 계산
	CalculateForce(AverageAcceleration);

	GEngine->AddOnScreenDebugMessage(1, DeltaTime, FColor::Blue, Force.ToString());
}

void ABat::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(ABall* Ball = Cast<ABall>(OtherActor))
	{
		Ball->HitBat(Force);
	}
}

FVector ABat::CalculateAverage(const TArray<FVector>& Vectors)
{
	FVector Sum = FVector::ZeroVector;
	for (const FVector& Vec : Vectors)
	{
		Sum += Vec;
	}
	return Sum / Vectors.Num();
}

void ABat::CalculateForce(const FVector& AverageAcceleration)
{
	Force = Mass * AverageAcceleration;

}
