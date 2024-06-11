#include "Actor/Pawn/Enemy/Shark.h"
#include "Curves/CurveFloat.h"

AShark::AShark()
{
	PrimaryActorTick.bCanEverTick = true;

	Head = CreateDefaultSubobject<UStaticMeshComponent>("Head");
	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");

	Body->SetRelativeRotation(FRotator(0, 180, 0));
	RootComponent = Head;
	Body->SetupAttachment(Head);

	{
		ConstructorHelpers::FObjectFinder<UCurveFloat> Asset(TEXT("/Script/Engine.CurveFloat'/Game/_Dev/Curve/Shark.Shark'"));
		if (Asset.Succeeded())
			Curve = Asset.Object;
	}
	CurrentAngle = 0.0f;
}

void AShark::BeginPlay()
{
	Super::BeginPlay();

	{
		TimelineFloat.BindUFunction(this, "SharkMove");
		TimelineFinished.BindUFunction(this, FName("TimelineHandle"));
		Timeline.AddInterpFloat(Curve, TimelineFloat);
		Timeline.SetPlayRate(1.f);
		Timeline.SetTimelineFinishedFunc(TimelineFinished);
		Timeline.PlayFromStart();
	}
}

void AShark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);

	if (CenterActor)
	{
		// 각도를 증가시킴
		CurrentAngle += RotationSpeed * DeltaTime;

		// 각도를 라디안으로 변환
		float Radians = FMath::DegreesToRadians(CurrentAngle);

		// 중심 Actor의 위치를 기준으로 원형 궤도의 새로운 위치 계산
		FVector NewLocation = CenterActor->GetActorLocation();
		NewLocation.X += CircleRadius * FMath::Cos(Radians);
		NewLocation.Y += CircleRadius * FMath::Sin(Radians);

		// 상어의 위치 업데이트
		SetActorLocation(NewLocation);

		// 상어가 중심을 향하도록 회전
		FRotator NewRotation = (CenterActor->GetActorLocation() - GetActorLocation()).Rotation();
		SetActorRotation(NewRotation);
	}
}

void AShark::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AShark::SharkMove(float Infloat)
{
	float Yawfloat = FMath::Lerp(160, 200, Infloat);
	Body->SetRelativeRotation(FRotator(0, Yawfloat, 0).Quaternion());
}

void AShark::TimelineHandle()
{
	if (bFlipFlop)
	{
		bFlipFlop = false;
		Timeline.ReverseFromEnd();
	}
	else
	{
		bFlipFlop = true;
		Timeline.PlayFromStart();
	}
}

