#include "Actor/Player/HandGraph.h"
#include "Actor/Player/VRCharacter.h"

UVRHandsInputDataConfig::UVRHandsInputDataConfig()
{
}


UVRHandsAnimationInputDataConfig::UVRHandsAnimationInputDataConfig()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/_Dev/Hands/Input/IMC_Hands.IMC_Hands'") };
		check(Asset.Object);
		InputMappingContext = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Hands/Input/IA_LeftGrab.IA_LeftGrab'") };
		check(Asset.Object);
		IA_Grab_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Hands/Input/IA_RightGrab.IA_RightGrab'") };
		check(Asset.Object);
		IA_Grab_Right = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Hands/Input/IA_LeftPoint.IA_LeftPoint'") };
		check(Asset.Object);
		IA_Point_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Hands/Input/IA_RightPoint.IA_RightPoint'") };
		check(Asset.Object);
		IA_Point_Right = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Hands/Input/IA_LeftCurl.IA_LeftCurl'") };
		check(Asset.Object);
		IA_IndexCurl_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Hands/Input/IA_RightCurl.IA_RightCurl'") };
		check(Asset.Object);
		IA_IndexCurl_Right = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Hands/Input/IA_LeftThumb.IA_LeftThumb'") };
		check(Asset.Object);
		IA_Thumb_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Hands/Input/IA_RightThumb.IA_RightThumb'") };
		check(Asset.Object);
		IA_Thumb_Right = Asset.Object;
	}
}

void UHandGraph::SetupPlayerInputComponent(UMotionControllerComponent* InMotionControllerComponent, UEnhancedInputComponent* InputComponent)
{
	MotionControllerComponent = InMotionControllerComponent;
	check(MotionControllerComponent);

	AVRCharacter* Character = Cast<AVRCharacter>(MotionControllerComponent->GetOwner());
	check(Character);

	const UVRHandsAnimationInputDataConfig* VRInputDataConfig = GetDefault<UVRHandsAnimationInputDataConfig>();

	HandType = MotionControllerComponent->MotionSource == LeftGrip ? EHandType::Left : EHandType::Right;
	VRHandAnimInstance = HandType == EHandType::Left ?
		Cast<UVRAnimInstance>(Character->LeftHand->GetAnimInstance()) : Cast<UVRAnimInstance>(Character->RightHand->GetAnimInstance());
	check(VRHandAnimInstance);

	UInputAction* IA_Grab = HandType == EHandType::Left ? VRInputDataConfig->IA_Grab_Left : VRInputDataConfig->IA_Grab_Right;
	UInputAction* IA_Point = HandType == EHandType::Left ? VRInputDataConfig->IA_Point_Left : VRInputDataConfig->IA_Point_Right;
	UInputAction* IA_IndexCurl = HandType == EHandType::Left ? VRInputDataConfig->IA_IndexCurl_Left : VRInputDataConfig->IA_IndexCurl_Right;
	UInputAction* IA_Thumb = HandType == EHandType::Left ? VRInputDataConfig->IA_Thumb_Left : VRInputDataConfig->IA_Thumb_Right;

	InputComponent->BindAction(IA_Grab, ETriggerEvent::Triggered, this, &ThisClass::OnGrabTriggered);
	InputComponent->BindAction(IA_Grab, ETriggerEvent::Canceled, this, &ThisClass::OnGrabCompleted);
	InputComponent->BindAction(IA_Grab, ETriggerEvent::Completed, this, &ThisClass::OnGrabCompleted);

	InputComponent->BindAction(IA_Point, ETriggerEvent::Started, this, &ThisClass::OnPointStarted);
	InputComponent->BindAction(IA_Point, ETriggerEvent::Canceled, this, &ThisClass::OnPointStarted);
	InputComponent->BindAction(IA_Point, ETriggerEvent::Completed, this, &ThisClass::OnPointCompleted);

	InputComponent->BindAction(IA_IndexCurl, ETriggerEvent::Triggered, this, &ThisClass::OnIndexCurlTriggered);
	InputComponent->BindAction(IA_IndexCurl, ETriggerEvent::Canceled, this, &ThisClass::OnIndexCurlCompleted);
	InputComponent->BindAction(IA_IndexCurl, ETriggerEvent::Completed, this, &ThisClass::OnIndexCurlCompleted);

	InputComponent->BindAction(IA_Thumb, ETriggerEvent::Started, this, &ThisClass::OnThumbStarted);
	InputComponent->BindAction(IA_Thumb, ETriggerEvent::Canceled, this, &ThisClass::OnThumbStarted);
	InputComponent->BindAction(IA_Thumb, ETriggerEvent::Completed, this, &ThisClass::OnThumbCompleted);
}

void UHandGraph::OnGrabTriggered(const FInputActionValue& InputActionValue)
{
	const float ActionValue = InputActionValue.Get<float>();
	VRHandAnimInstance->PoseAlphaGrap = ActionValue;
}

void UHandGraph::OnGrabCompleted(const FInputActionValue& InputActionValue)
{
	VRHandAnimInstance->PoseAlphaGrap = 0.f;
}

void UHandGraph::OnPointStarted(const FInputActionValue& InputActionValue)
{
	VRHandAnimInstance->PoseAlphaPoint = 0.f;
}

void UHandGraph::OnPointCompleted(const FInputActionValue& InputActionValue)
{
	VRHandAnimInstance->PoseAlphaPoint = 1.f;
}

void UHandGraph::OnIndexCurlTriggered(const FInputActionValue& InputActionValue)
{
	const float ActionValue = InputActionValue.Get<float>();
	VRHandAnimInstance->PoseAlphaIndexCurl = ActionValue;
}

void UHandGraph::OnIndexCurlCompleted(const FInputActionValue& InputActionValue)
{
	VRHandAnimInstance->PoseAlphaIndexCurl = 0.f;
}

void UHandGraph::OnThumbStarted(const FInputActionValue& InputActionValue)
{
	VRHandAnimInstance->PoseAlphaThumb = 0.f;
}

void UHandGraph::OnThumbCompleted(const FInputActionValue& InputActionValue)
{
	VRHandAnimInstance->PoseAlphaThumb = 1.f;
}

