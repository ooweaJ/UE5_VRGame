#include "Actor/Player/VRCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Misc/NameTable.h"
#include "Data/Input/BasicInputDataConfig.h"
#include "Kismet/KismetMathLibrary.h"

AVRCharacter::AVRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetCollisionProfileName("Pawn");

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(GetRootComponent());

	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));

	MotionControllerLeft->SetTrackingMotionSource(LeftGrip);
	MotionControllerRight->SetTrackingMotionSource(RightGrip);
	MotionControllerLeft->SetupAttachment(GetRootComponent());
	MotionControllerRight->SetupAttachment(GetRootComponent());

	HandGraphLeft = CreateDefaultSubobject<UHandGraph>(TEXT("HandGraphLeft"));
	HandGraphRight = CreateDefaultSubobject<UHandGraph>(TEXT("HandGraphRight"));

	GetMesh()->DestroyComponent();

	LeftHand = CreateDefaultSubobject<UVRHandMeshComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(MotionControllerLeft);
	RightHand = CreateDefaultSubobject<UVRHandMeshComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(MotionControllerRight);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/_Dev/Hands/ABP_Hand.ABP_Hand_C'"));
	check(AnimClass.Class);

	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
		RightHand->SetSkeletalMeshAsset(Asset.Object);

		const FTransform Transform = FTransform(FRotator(25.0, 0.0, 90.0), FVector(-2.98, 3.5, 4.56));
		RightHand->SetRelativeTransform(Transform);
		RightHand->SetAnimClass(AnimClass.Class);
	}
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
		LeftHand->SetSkeletalMeshAsset(Asset.Object);

		const FTransform Transform = FTransform(FRotator(-25.0, 180.0, 90.0), FVector(-2.98, -3.5, 4.56));
		LeftHand->SetRelativeTransform(Transform);
		LeftHand->bMirror = true;
		LeftHand->SetAnimClass(AnimClass.Class);
	}
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

		const UBasicInputDataConfig* BasicInputDataConfig = GetDefault<UBasicInputDataConfig>();
		Subsystem->AddMappingContext(BasicInputDataConfig->InputMappingContext, 0);

		const UVRHandsAnimationInputDataConfig* VRInputDataConfig = GetDefault<UVRHandsAnimationInputDataConfig>();
		Subsystem->AddMappingContext(VRInputDataConfig->InputMappingContext, 0);
	}
	else { check(false); }
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	ensure(EnhancedInputComponent);

	{
		const UBasicInputDataConfig* BasicInputDataConfig = GetDefault<UBasicInputDataConfig>();
		EnhancedInputComponent->BindAction(BasicInputDataConfig->Move, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
	}
	{
		const UVRHandsInputDataConfig* VRHandsInputDataConfig = GetDefault<UVRHandsInputDataConfig>();
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Left, ETriggerEvent::Started, this, &ThisClass::OnGrabLeftStarted);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Left, ETriggerEvent::Completed, this, &ThisClass::OnGrabLeftCompleted);

		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Right, ETriggerEvent::Started, this, &ThisClass::OnGrabRightStarted);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Right, ETriggerEvent::Completed, this, &ThisClass::OnGrabRightCompleted);
	}
	{
		HandGraphLeft->SetupPlayerInputComponent(MotionControllerLeft, EnhancedInputComponent);
		HandGraphRight->SetupPlayerInputComponent(MotionControllerRight, EnhancedInputComponent);
	}
}


void AVRCharacter::OnMove(const FInputActionValue& InputActionValue)
{
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

	const FRotator CameraRotator = VRCamera->GetRelativeRotation();
	const FRotator CameraYawRotator = FRotator(0., CameraRotator.Yaw, 0.);

	if (!FMath::IsNearlyZero(ActionValue.Y))
	{
		const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(CameraYawRotator);
		AddMovementInput(ForwardVector, ActionValue.Y);
	}

	if (!FMath::IsNearlyZero(ActionValue.X))
	{
		const FVector RightVector = UKismetMathLibrary::GetRightVector(CameraYawRotator);
		AddMovementInput(RightVector, ActionValue.X);
	}
}

void AVRCharacter::OnGrabStarted(UMotionControllerComponent* MotionControllerComponent, const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("OnGrabStarted"));
}

void AVRCharacter::OnGrabCompleted(UMotionControllerComponent* MotionControllerComponent, const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("OnGrabCompleted"));
}
