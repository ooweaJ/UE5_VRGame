#include "Actor/Player/VRCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Misc/NameTable.h"
#include "Data/Input/BasicInputDataConfig.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actor/Pawn/SubMarine.h"

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

	LeftInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("LeftInteraction"));
	RightInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("RightInteraction"));

	LeftInteraction->SetupAttachment(MotionControllerLeft);
	RightInteraction->SetupAttachment(MotionControllerRight);

	LeftInteraction->bShowDebug = true;
	LeftInteraction->TraceChannel = FCollisionChannel::WorldUI;
	RightInteraction->bShowDebug = true;
	RightInteraction->TraceChannel = FCollisionChannel::WorldUI;
}

void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

		const UBasicInputDataConfig* BasicInputDataConfig = GetDefault<UBasicInputDataConfig>();
		Subsystem->AddMappingContext(BasicInputDataConfig->InputMappingContext, 0);

		const UVRHandsInputDataConfig* VRHandsInputDataConfig = GetDefault<UVRHandsInputDataConfig>();
		Subsystem->AddMappingContext(VRHandsInputDataConfig->InputMappingContext, 0);

		// HandGraphLeft, HandGraphRight
		const UVRHandsAnimationInputDataConfig* VRHandsAnimationInputDataConfig = GetDefault<UVRHandsAnimationInputDataConfig>();
		Subsystem->AddMappingContext(VRHandsAnimationInputDataConfig->InputMappingContext, 1);
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
		EnhancedInputComponent->BindAction(BasicInputDataConfig->Move, ETriggerEvent::Completed, this, &ThisClass::OffMove);
		EnhancedInputComponent->BindAction(BasicInputDataConfig->Look, ETriggerEvent::Triggered, this, &ThisClass::OnLook);
		EnhancedInputComponent->BindAction(BasicInputDataConfig->Look, ETriggerEvent::Completed, this, &ThisClass::OffLook);
	}
	{
		const UVRHandsInputDataConfig* VRHandsInputDataConfig = GetDefault<UVRHandsInputDataConfig>();
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Left, ETriggerEvent::Triggered, this, &ThisClass::OnGrabLeftStarted);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Left, ETriggerEvent::Completed, this, &ThisClass::OnGrabLeftCompleted);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Right, ETriggerEvent::Triggered, this, &ThisClass::OnGrabRightStarted);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Right, ETriggerEvent::Completed, this, &ThisClass::OnGrabRightCompleted);

		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_IndexCurl_Left, ETriggerEvent::Triggered, this, &ThisClass::OnLeftIndexTriggered);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_IndexCurl_Left, ETriggerEvent::Completed, this, &ThisClass::OnLeftIndexCompleted);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_IndexCurl_Right, ETriggerEvent::Triggered, this, &ThisClass::OnRightIndexTriggered);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_IndexCurl_Right, ETriggerEvent::Completed, this, &ThisClass::OnRightIndexCompleted);

		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_A, ETriggerEvent::Triggered, this, &ThisClass::OnATriggered);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_A, ETriggerEvent::Completed, this, &ThisClass::OnACompleted);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_B, ETriggerEvent::Triggered, this, &ThisClass::OnBTriggered);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_B, ETriggerEvent::Completed, this, &ThisClass::OnBCompleted);

	}
	{
		HandGraphLeft->SetupPlayerInputComponent(MotionControllerLeft, EnhancedInputComponent);
		HandGraphRight->SetupPlayerInputComponent(MotionControllerRight, EnhancedInputComponent);
	}
}


void AVRCharacter::OnMove(const FInputActionValue& InputActionValue)
{

	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
	if (bRiding)
	{
		if (!FMath::IsNearlyZero(ActionValue.X))
		{
			RidingSubMarine->CalculateSteering(ActionValue.X);
			RidingSubMarine->OnSteeringStart();
		}
	}
	else
	{
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
}

void AVRCharacter::OffMove(const FInputActionValue& InputActionValue)
{
	if (bRiding)
	{
		RidingSubMarine->OnSteeringStop();
	}
}

void AVRCharacter::OnLook(const FInputActionValue& InputActionValue)
{
	const FVector ActionValue = InputActionValue.Get<FVector>();
	AddControllerYawInput(ActionValue.X);
	AddControllerPitchInput(ActionValue.Y);
}

void AVRCharacter::OffLook(const FInputActionValue& InputActionValue)
{

}

void AVRCharacter::OnGrabStarted(UMotionControllerComponent* MotionControllerComponent, const FInputActionValue& InputActionValue)
{
	if (MotionControllerComponent == MotionControllerLeft)
	{
		if (bRiding)
		{
			RidingSubMarine->InputUPDown(-(InputActionValue.Get<float>()));
		}
	}
	else
	{
		if (bRiding)
		{
			RidingSubMarine->InputUPDown(InputActionValue.Get<float>());
		}
	}
}

void AVRCharacter::OnGrabCompleted(UMotionControllerComponent* MotionControllerComponent, const FInputActionValue& InputActionValue)
{
	if (bRiding)
	{
		RidingSubMarine->ZEngineOff();
	}
}

void AVRCharacter::OnLeftIndexTriggered(const FInputActionValue& InputActionValue)
{
	if (bRiding)
	{
		RidingSubMarine->BulletFire();
	}
	LeftInteraction->PressPointerKey(EKeys::LeftMouseButton);
}

void AVRCharacter::OnLeftIndexCompleted(const FInputActionValue& InputActionValue)
{
	LeftInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
}

void AVRCharacter::OnRightIndexTriggered(const FInputActionValue& InputActionValue)
{
	RightInteraction->PressPointerKey(EKeys::LeftMouseButton);
}

void AVRCharacter::OnRightIndexCompleted(const FInputActionValue& InputActionValue)
{
	RightInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
}

void AVRCharacter::OnATriggered(const FInputActionValue& InputActionValue)
{
	if (bRiding)
	{
		RidingSubMarine->InputThrottle(-(InputActionValue.Get<float>()));
	}
}

void AVRCharacter::OnBTriggered(const FInputActionValue& InputActionValue)
{
	if (bRiding)
	{
		RidingSubMarine->InputThrottle((InputActionValue.Get<float>()));
	}
}

void AVRCharacter::OnACompleted(const FInputActionValue& InputActionValue)
{
	if (bRiding)
	{
		RidingSubMarine->InputThrottle(0.f);
	}
}

void AVRCharacter::OnBCompleted(const FInputActionValue& InputActionValue)
{
	if (bRiding)
	{
		RidingSubMarine->InputThrottle(0.f);
	}
}

void AVRCharacter::OnRiding(ASubMarine* RidingPawn)
{
	bRiding = true;
	RidingSubMarine = RidingPawn;
}

void AVRCharacter::OffRiding()
{
	bRiding = false;
	RidingSubMarine = nullptr;
}
