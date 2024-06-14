#include "Actor/Pawn/MenuPawn.h"
#include "Data/Input/BasicInputDataConfig.h"

AMenuPawn::AMenuPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(GetRootComponent());

	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
	MotionControllerRight->SetupAttachment(GetRootComponent());

	RightInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("RightInteraction"));
	RightInteraction->SetupAttachment(MotionControllerRight);
	RightInteraction->bShowDebug = true;
}

void AMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

		const UVRHandsInputDataConfig* VRHandsInputDataConfig = GetDefault<UVRHandsInputDataConfig>();
		Subsystem->AddMappingContext(VRHandsInputDataConfig->InputMappingContext, 0);
	}
}

void AMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	ensure(EnhancedInputComponent);
	const UVRHandsInputDataConfig* VRHandsInputDataConfig = GetDefault<UVRHandsInputDataConfig>();
	EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_IndexCurl_Right, ETriggerEvent::Triggered, this, &ThisClass::OnRightIndexTriggered);
	EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_IndexCurl_Right, ETriggerEvent::Completed, this, &ThisClass::OnRightIndexCompleted);
}

void AMenuPawn::OnRightIndexTriggered(const FInputActionValue& InputActionValue)
{
	RightInteraction->PressPointerKey(EKeys::LeftMouseButton);
}

void AMenuPawn::OnRightIndexCompleted(const FInputActionValue& InputActionValue)
{
	RightInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
}

