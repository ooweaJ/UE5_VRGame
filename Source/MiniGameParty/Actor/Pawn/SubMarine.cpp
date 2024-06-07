#include "Actor/Pawn/SubMarine.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/UI_SubMarine.h"
#include "Misc/Misc.h"

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
		SubMarineMovementComponent->InputVector(GetActorForwardVector());
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

