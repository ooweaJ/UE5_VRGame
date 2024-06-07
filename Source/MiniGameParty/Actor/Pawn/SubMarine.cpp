#include "Actor/Pawn/SubMarine.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/UI_SubMarine.h"
#include "Misc/Misc.h"
#include "Actor/Player/VRCharacter.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

ASubMarine::ASubMarine()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(250.f);
	Sphere->SetCollisionProfileName(FCollisionPresetNameTable::Static);
	RootComponent = Sphere;

	SubMarineMovementComponent = CreateDefaultSubobject<USubMarineMovementComponent>(TEXT("SubMarineMovementComponent"));
	SubMarineMovementComponent->UpdatedComponent = RootComponent;
	SubMarineMovementComponent->SetPawnOwner(this);

	SubMarine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubMarine"));
	SubMarine->SetupAttachment(RootComponent);
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> Asset (TEXT("/Script/Engine.StaticMesh'/Game/_Dev/StaticMesh/SM_SubMarine.SM_SubMarine'"));
		if (Asset.Succeeded())
		{
			SubMarine->SetStaticMesh(Asset.Object);
			SubMarine->SetRelativeRotation(FRotator(0, 140, 0));
		}
	}
	SteeringOffSet = CreateDefaultSubobject<USceneComponent>(TEXT("SteeringOffSet"));
	SteeringOffSet->SetupAttachment(RootComponent);

	SteeringWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SteeringWheel"));
	SteeringWheel->SetupAttachment(SteeringOffSet);
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.StaticMesh'/Game/Megascans/3D_Assets/Steering_Wheel_wcvodcw/S_Steering_Wheel_wcvodcw_lod3_Var1.S_Steering_Wheel_wcvodcw_lod3_Var1'"));
		if (Asset.Succeeded())
		{
			SteeringWheel->SetStaticMesh(Asset.Object);
			SteeringWheel->SetRelativeRotation(FRotator(90, 0, 90));
		}
	}

	{
		ConstructorHelpers::FClassFinder<UUserWidget> Class(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_Dev/UI/UI_SubMarine.UI_SubMarine_C'"));
		if (!Class.Succeeded()) return;

		Menu = CreateDefaultSubobject<UWidgetComponent>(TEXT("Menu"));
		Menu->SetupAttachment(RootComponent);
		FTransform Transform;
		Transform.SetLocation(FVector(77., -150., -122.));
		Transform.SetRotation(FRotator(20.0, 110.0, 0.0).Quaternion());
		Transform.SetScale3D(FVector(0.5, 0.5, 0.5));
		Menu->SetRelativeTransform(Transform);
		Menu->SetWidgetClass(Class.Class);
		Menu->SetDrawSize(FVector2D(190, 205));
	}
}

void ASubMarine::BeginPlay()
{
	Super::BeginPlay();

	UUI_SubMarine* Widget = Cast<UUI_SubMarine>(Menu->GetUserWidgetObject());
	if (Widget)
	{
		SubMarineMenu = Widget;
		SubMarineMenu->Gear->OnClicked.AddDynamic(this, &ThisClass::OnGearBtnClicked);
		SubMarineMenu->Gear2->OnClicked.AddDynamic(this, &ThisClass::OnGear2BtnClicked);
		SubMarineMenu->Gear3->OnClicked.AddDynamic(this, &ThisClass::OnGear3BtnClicked);
		SubMarineMenu->Riding->OnClicked.AddDynamic(this, &ThisClass::OnRidingBtnClicked);
		SubMarineMenu->Auto->OnClicked.AddDynamic(this, &ThisClass::OnAutoBtnClicked);
	}
}

void ASubMarine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bAuto)
	{
		SubMarineMovementComponent->InputVector(GetActorForwardVector() * EnginePower * DeltaTime);
	}

	if (bIsSteering)
	{
		CalculateSteering();
		AddYaw();
	}
	else
	{
		FRotator TargetRotation = FRotator(0, 90, 90);
		FRotator CurrentRotation = SteeringWheel->GetRelativeRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f);
		SteeringWheel->SetRelativeRotation(NewRotation);
	}
}

void ASubMarine::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASubMarine::OnAuto()
{
	if (bAuto)
	{
		bAuto = false;
		SubMarineMenu->SelectAuto(bAuto);
	}
	else
	{
		bAuto = true;
		SubMarineMenu->SelectAuto(bAuto);
	}
}

void ASubMarine::OnRiding()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		AVRCharacter* VRCharacter = Cast<AVRCharacter>(PlayerController->GetCharacter());
		VRCharacter->OnRiding(this);
		RidingCharacter = VRCharacter;
	}
}

void ASubMarine::InputThrottle(float Throttle)
{
	FVector InputVector = GetActorForwardVector() * Throttle * EnginePower;
	SubMarineMovementComponent->InputVector(InputVector);
}

void ASubMarine::InputUPDown(float Value)
{
	FVector InputVector = GetActorUpVector() * Value * EnginePower;
	SubMarineMovementComponent->InputVector(InputVector);
}

void ASubMarine::OnGearBtnClicked() { ChangeGear(ESubmarineGear::Gear1); }
void ASubMarine::OnGear2BtnClicked() { ChangeGear(ESubmarineGear::Gear2); }
void ASubMarine::OnGear3BtnClicked() { ChangeGear(ESubmarineGear::Gear3); }
void ASubMarine::OnRidingBtnClicked() { OnRiding(); }
void ASubMarine::OnAutoBtnClicked() { OnAuto(); }

void ASubMarine::ChangeGear(ESubmarineGear InputGear)
{
	ESubmarineGear CurrentGear = SubMarineMovementComponent->GetCurrentGear();
	if (CurrentGear == InputGear) return;
	else
	{
		SubMarineMovementComponent->UpdateMaxSpeed(InputGear);
		SubMarineMenu->SelectGear((int32)InputGear);
	}
}

void ASubMarine::AddYaw()
{
	float YawAngle = SteeringWheel->GetRelativeRotation().Pitch;
	float AddYaw = YawAngle / -60.f;
	FRotator AddRotator = FRotator(0, AddYaw, 0);
	AddActorWorldRotation(AddRotator.Quaternion());
}

void ASubMarine::OnSteeringStart()
{
	bIsSteering = true;
	InitialLeftControllerPosition = RidingCharacter->MotionControllerLeft->GetComponentLocation();
	InitialRightControllerPosition = RidingCharacter->MotionControllerRight->GetComponentLocation();
}

void ASubMarine::OnSteeringStop()
{
	bIsSteering = false;
}

void ASubMarine::CalculateSteering()
{
	// 현재 컨트롤러 위치 가져오기
	FVector CurrentLeftControllerPosition = RidingCharacter->MotionControllerLeft->GetComponentLocation();
	FVector CurrentRightControllerPosition = RidingCharacter->MotionControllerRight->GetComponentLocation();

	// 컨트롤러의 이동 벡터 계산
	FVector LeftMovement = CurrentLeftControllerPosition - InitialLeftControllerPosition;
	FVector RightMovement = CurrentRightControllerPosition - InitialRightControllerPosition;

	// 양손의 이동값을 평균하여 계산
	float AverageMovementZ = (RightMovement.Z - LeftMovement.Z) / 2.0f;
	float AverageMovementY = (LeftMovement.Y - RightMovement.Y) / 2.0f;

	float SteeringAmount;
	if(AverageMovementZ > 0)
		SteeringAmount = AverageMovementZ + AverageMovementY;
	else
		SteeringAmount = AverageMovementZ - AverageMovementY;

	// 평균값을 이용하여 회전값 계산
	FRotator AverageRotation = FRotator(0,0, SteeringAmount * -SteeringSensitivity);

	// 운전대에 상대적 회전 적용
	SteeringWheel->AddRelativeRotation(AverageRotation.Quaternion());

	float SteeringWheelPitch = SteeringWheel->GetRelativeRotation().Pitch;

	if (SteeringWheelPitch > 60.f)
	{
		FRotator NewRotator = FRotator(60, 90, 90);
		SteeringWheel->SetRelativeRotation(NewRotator);
	}

	if (SteeringWheelPitch < -60.f)
	{
		FRotator NewRotator = FRotator(-60, 90, 90);
		SteeringWheel->SetRelativeRotation(NewRotator);
	}
}

