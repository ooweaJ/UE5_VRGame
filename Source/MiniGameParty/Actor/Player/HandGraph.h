#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "Components/VRHandMeshComponent.h"
#include "Animation/VRAnimInstance.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "HandGraph.generated.h"

UCLASS()
class MINIGAMEPARTY_API UVRHandsAnimationInputDataConfig : public UObject
{
	GENERATED_BODY()

public:
	UVRHandsAnimationInputDataConfig();

	UInputMappingContext* InputMappingContext = nullptr;

public:
	UInputAction* IA_Grab_Left = nullptr;
	UInputAction* IA_Grab_Right = nullptr;

	UInputAction* IA_Point_Left = nullptr;
	UInputAction* IA_Point_Right = nullptr;

	UInputAction* IA_IndexCurl_Left = nullptr;
	UInputAction* IA_IndexCurl_Right = nullptr;

	UInputAction* IA_Thumb_Left = nullptr;
	UInputAction* IA_Thumb_Right = nullptr;
};

UCLASS()
class MINIGAMEPARTY_API UVRHandsInputDataConfig : public UObject
{
	GENERATED_BODY()

public:
	UVRHandsInputDataConfig();

	UInputMappingContext* InputMappingContext = nullptr;

public:
	UInputAction* IA_Grab_Left = nullptr;
	UInputAction* IA_Grab_Right = nullptr;

	UInputAction* IA_IndexCurl_Left = nullptr;
	UInputAction* IA_IndexCurl_Right = nullptr;

	UInputAction* IA_A = nullptr;
	UInputAction* IA_B = nullptr;
	/*UInputAction* IA_Point_Left = nullptr;
	UInputAction* IA_Point_Right = nullptr;

	

	UInputAction* IA_Thumb_Left = nullptr;
	UInputAction* IA_Thumb_Right = nullptr;*/
};


UCLASS()
class  UHandGraph : public UActorComponent
{
	GENERATED_BODY()

	enum EHandType
	{
		Left,
		Right
	};

public:
	void SetupPlayerInputComponent(UMotionControllerComponent* InMotionControllerComponent, UEnhancedInputComponent* InputComponent);
	void OnGrabTriggered(const FInputActionValue& InputActionValue);
	void OnGrabCompleted(const FInputActionValue& InputActionValue);

	void OnPointStarted(const FInputActionValue& InputActionValue);
	void OnPointCompleted(const FInputActionValue& InputActionValue);

	void OnIndexCurlTriggered(const FInputActionValue& InputActionValue);
	void OnIndexCurlCompleted(const FInputActionValue& InputActionValue);

	void OnThumbStarted(const FInputActionValue& InputActionValue);
	void OnThumbCompleted(const FInputActionValue& InputActionValue);

	UMotionControllerComponent* MotionControllerComponent = nullptr;
	UVRAnimInstance* VRHandAnimInstance = nullptr;
	EHandType HandType = (EHandType)-1;
	
};
