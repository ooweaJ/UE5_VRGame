#include "Actor/Pawn/SubMarine.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/UI_SubMarine.h"
#include "Misc/Misc.h"
#include "Actor/Player/VRCharacter.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actor/Projectile/Bullet.h"
#include "Engine.h"
#include "Materials/MaterialInstanceDynamic.h"

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

	SteeringWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SteeringWheel"));
	SteeringWheel->SetupAttachment(RootComponent);
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.StaticMesh'/Game/Megascans/3D_Assets/Steering_Wheel_wcvodcw/S_Steering_Wheel_wcvodcw_lod3_Var1.S_Steering_Wheel_wcvodcw_lod3_Var1'"));
		if (Asset.Succeeded())
		{
			SteeringWheel->SetStaticMesh(Asset.Object);
			SteeringWheel->SetRelativeRotation(FRotator(90, 0, 50));
		}
	}


	Muzzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(RootComponent);
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
		if (Asset.Succeeded())
		{
			Muzzle->SetStaticMesh(Asset.Object);
			Muzzle->SetRelativeRotation(FRotator(-90, 0, 0));
			Muzzle->SetRelativeLocation(FVector(300, 0, -140));
			Muzzle->SetRelativeScale3D(FVector(0.3, 0.3, 1.75));
		}
	}
	MuzzleOffSet = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleOffSet"));
	MuzzleOffSet->SetupAttachment(Muzzle);
	MuzzleOffSet->SetRelativeLocation(FVector(0, 0, 51));
	{
		ConstructorHelpers::FClassFinder<UUserWidget> Class(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_Dev/UI/UI_SubMarine.UI_SubMarine_C'"));
		if (!Class.Succeeded()) return;

		Menu = CreateDefaultSubobject<UWidgetComponent>(TEXT("Menu"));
		Menu->SetupAttachment(RootComponent);
		FTransform Transform;
		Transform.SetLocation(FVector(145, -88, -122));
		Transform.SetRotation(FRotator(0, 29.5, 148.5).Quaternion());
		Transform.SetScale3D(FVector(0.5, 0.5, 0.5));
		Menu->SetRelativeTransform(Transform);
		Menu->SetWidgetClass(Class.Class);
		Menu->SetDrawSize(FVector2D(190, 205));
	}

	{
		ConstructorHelpers::FClassFinder<ABullet> Class(TEXT("/Script/Engine.Blueprint'/Game/_Dev/Player/Bullet.Bullet_C'"));
		if (Class.Succeeded())
			BulletClass = Class.Class;
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

	if (SubMarine && SubMarine->GetStaticMesh())
	{
		UMaterialInterface* Material = SubMarine->GetMaterial(1);
		if (Material)
		{
			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
			if (DynamicMaterialInstance)
			{
				SubMarine->SetMaterial(1, DynamicMaterialInstance);
			}
		}
	}
}

float ASubMarine::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Window += FVector(1, 1, 1);
	Hit(Window);
	return 0.0f;
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
		AddYaw();
	}
	else
	{
		FRotator TargetRotation = FRotator(0, 90, 90);
		FRotator CurrentRotation = SteeringWheel->GetRelativeRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f);
		SteeringWheel->SetRelativeRotation(NewRotation);
		AddYaw();
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
		SubMarineMenu->OnRiding();
	}
}

void ASubMarine::InputThrottle(float Throttle)
{
	FVector InputVector = GetActorForwardVector() * Throttle;
	SubMarineMovementComponent->InputVector(InputVector);
	SubMarineMovementComponent->SetThrottle(Throttle);
}

void ASubMarine::InputUPDown(float Throttle)
{
	FVector InputVector = GetActorUpVector() * Throttle;
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

void ASubMarine::Hit(FVector InVector)
{
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetVectorParameterValue("Window", InVector);
	}
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

void ASubMarine::OnFire()
{
	bFire = false;
}

void ASubMarine::CalculateSteering(float InputValue)
{
	FRotator AverageRotation = FRotator(0, 0, InputValue);

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

void ASubMarine::ZEngineOff()
{
	SubMarineMovementComponent->Velocity.Z = 0.f;
}

void ASubMarine::BulletFire()
{
	if (bFire) return;
	bFire = true;
	FTransform DefaultTransform;
	DefaultTransform.SetLocation(MuzzleOffSet->GetComponentLocation());

	ABullet* SpawnedActor = GetWorld()->SpawnActorDeferred<ABullet>(BulletClass, DefaultTransform, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	SpawnedActor->SetVelocity(GetActorForwardVector());
	SpawnedActor->FinishSpawning(DefaultTransform, true);
	if (SpawnSound)
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SpawnSound, MuzzleOffSet->GetComponentLocation());
	UKismetSystemLibrary::K2_SetTimer(this, "OnFire", FireRate, false);
}

