#include "Actor/Pawn/Enemy/Shark.h"
#include "Curves/CurveFloat.h"
#include "Actor/Pawn/SubMarine.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/Controller/SharkAIController.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"

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

	{
		ConstructorHelpers::FClassFinder<UCameraShakeBase> Class(TEXT("/Script/Engine.Blueprint'/Game/_Dev/SharkAttack.SharkAttack_C'"));
		if (Class.Succeeded())
			CameraShake = Class.Class;
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

	Head->OnComponentHit.AddDynamic(this, &ThisClass::OnComponentHit);
	ReturnLocation = GetActorLocation();
}

void AShark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);

	if(bAttack)
		FloatingPawnMovement->Velocity = TargetDirection * 1500.f;

	if(bStrafe)
		TargetStrafe(DeltaTime);

	if (CenterActor == nullptr)
		Return(DeltaTime);

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

void AShark::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bAttack) return;
	ASubMarine* SubMarine = Cast<ASubMarine>(OtherActor);

	if (!!ImpactParticle)
	{
		FVector location = Hit.Location;
		FRotator Rotation;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticle, location, Rotation);
	}

	if (SubMarine)
	{
		FDamageEvent de;
		SubMarine->TakeDamage(1, de, GetController(), this);
		SubMarine->SubMarineMovementComponent->Velocity = FloatingPawnMovement->Velocity;

		if (!!CameraShake)
		{
			APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			controller->PlayerCameraManager->StartCameraShake(CameraShake);
		}
	}

	EndAttack();
}

void AShark::EndAttack()
{
	bAttack = false;
	FloatingPawnMovement->Velocity = FVector::ZeroVector;
	ASharkAIController* controller = Cast<ASharkAIController>(GetController());
	controller->SetAttack(bAttack);
}

void AShark::TargetStrafe(float DeltaTime)
{
	if (CenterActor)
	{
		CurrentAngle += RotationSpeed * DeltaTime;

		// 각도를 라디안으로 변환
		float Radians = FMath::DegreesToRadians(CurrentAngle);

		// 중심 Actor의 위치를 기준으로 원형 궤도의 새로운 위치 계산
		FVector CenterLocation = CenterActor->GetActorLocation();
		FVector TargetLocation = CenterLocation;
		TargetLocation.X += CircleRadius * FMath::Cos(Radians);
		TargetLocation.Y += CircleRadius * FMath::Sin(Radians);

		// 위치를 보간하여 업데이트
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, 5.0f); // 5.0f는 보간 속도, 필요에 따라 조정

		// 상어가 중심을 향하도록 회전
		FRotator TargetRotation = (CenterLocation - NewLocation).Rotation();

		// 회전을 보간하여 업데이트
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f); // 5.0f는 보간 속도, 필요에 따라 조정

		// 상어의 위치와 회전 업데이트
		SetActorLocation(NewLocation);
		SetActorRotation(NewRotation);
	}
}

void AShark::Targetbutt()
{
	bStrafe = false;
	bAttack = true;

	if (CenterActor)
	{
		ASharkAIController* controller = Cast<ASharkAIController>(GetController());
		controller->SetAttack(bAttack);
		TargetDirection = (CenterActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	}
}

void AShark::OnStrafe()
{
	// 현재 위치 가져오기
	FVector CurrentLocation = GetActorLocation();
	FVector CenterLocation = CenterActor->GetActorLocation();

	// 중심 위치로부터 현재 위치의 각도 계산
	FVector Direction = CurrentLocation - CenterLocation;
	float InitialAngle = FMath::Atan2(Direction.Y, Direction.X);

	// 현재 각도 업데이트
	CurrentAngle = FMath::RadiansToDegrees(InitialAngle);
	bStrafe = true;
	int32 RandomTime = FMath::RandRange(1, 5);
	UKismetSystemLibrary::K2_SetTimer(this, "Targetbutt", RandomTime, false);

}

void AShark::Return(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();

	// 위치를 보간하여 업데이트
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, ReturnLocation, DeltaTime, 2);

	// 새로운 위치 설정
	SetActorLocation(NewLocation);
}
