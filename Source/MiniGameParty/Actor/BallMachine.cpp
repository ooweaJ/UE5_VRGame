#include "Actor/BallMachine.h"
#include "Actor/Projectile/Ball.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Widgets/UI_Distance.h"

ABallMachine::ABallMachine()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(Mesh);

	{
		ConstructorHelpers::FClassFinder<ABall> Class(TEXT("/Script/Engine.Blueprint'/Game/_Dev/Actor/BP_Ball.BP_Ball_C'"));
		if (Class.Succeeded())
		{
			BallClass = Class.Class;
		}
	}

	{
		ConstructorHelpers::FClassFinder<UUserWidget> Class(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_Dev/UI/UI_Score.UI_Score_C'"));
		if (!Class.Succeeded()) return;

		Score = CreateDefaultSubobject<UWidgetComponent>(TEXT("Score"));
		Score->SetupAttachment(RootComponent);
		FTransform Transform;
		Transform.SetLocation(FVector(0, 0, 250));
		Score->SetRelativeTransform(Transform);
		Score->SetWidgetClass(Class.Class);
	}
}

void ABallMachine::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::SpawnBall, 5.0f, true);

	Distance = Cast<UUI_Distance>(Score->GetUserWidgetObject());
}

void ABallMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABallMachine::SpawnBall()
{
	if (BallClass)
	{
		FTransform DefaultTransform;
		FVector SpawnLocation = SpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = SpawnPoint->GetComponentRotation();
		DefaultTransform.SetLocation(SpawnLocation);
		DefaultTransform.SetRotation(SpawnRotation.Quaternion());

		if (SpawnParicle)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnParicle, SpawnLocation, SpawnRotation, true);

		if (SpawnSound)
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SpawnSound, SpawnLocation);

		ABall* Ball = GetWorld()->SpawnActorDeferred<ABall>(BallClass, DefaultTransform, this);
		Ball->SetSpeed();
		Ball->FinishSpawning(DefaultTransform, true);
	}
}

void ABallMachine::SetDistnace(float Input)
{
	Distance->SetDistance(Input);
}

