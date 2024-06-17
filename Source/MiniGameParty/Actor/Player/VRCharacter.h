// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/VRHandMeshComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Animation/VRAnimInstance.h"
#include "Actor/Player/HandGraph.h"
#include "VRCharacter.generated.h"

static inline const FName LeftGrip = TEXT("LeftGrip");
static inline const FName RightGrip = TEXT("RightGrip");

UCLASS()
class MINIGAMEPARTY_API AVRCharacter : public ACharacter
{
	friend class UHandGraph;
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnGrabLeftStarted(const FInputActionValue& InputActionValue) { OnGrabStarted(MotionControllerLeft, InputActionValue); }
	void OnGrabRightStarted(const FInputActionValue& InputActionValue) { OnGrabStarted(MotionControllerRight, InputActionValue); }
	void OnGrabStarted(UMotionControllerComponent* MotionControllerComponent, const FInputActionValue& InputActionValue);


	void OnGrabLeftTriggered(const FInputActionValue& InputActionValue) { OnGrabTriggered(MotionControllerLeft, InputActionValue); }
	void OnGrabRightTriggered(const FInputActionValue& InputActionValue) { OnGrabTriggered(MotionControllerRight, InputActionValue); }
	void OnGrabTriggered(UMotionControllerComponent* MotionControllerComponent, const FInputActionValue& InputActionValue);

	void OnGrabLeftCompleted(const FInputActionValue& InputActionValue) { OnGrabCompleted(MotionControllerLeft, InputActionValue); }
	void OnGrabRightCompleted(const FInputActionValue& InputActionValue) { OnGrabCompleted(MotionControllerRight, InputActionValue); }
	void OnGrabCompleted(UMotionControllerComponent* MotionControllerComponent, const FInputActionValue& InputActionValue);

	void OnLeftIndexTriggered(const FInputActionValue& InputActionValue);
	void OnLeftIndexCompleted(const FInputActionValue& InputActionValue);
	void OnRightIndexTriggered(const FInputActionValue& InputActionValue);
	void OnRightIndexCompleted(const FInputActionValue& InputActionValue);

	void OnATriggered(const FInputActionValue& InputActionValue);
	void OnBTriggered(const FInputActionValue& InputActionValue);
	void OnACompleted(const FInputActionValue& InputActionValue);
	void OnBCompleted(const FInputActionValue& InputActionValue);


public:
	void OnRiding(class ASubMarine* RidingPawn);
	void OffRiding();

protected:
	void OnMove(const FInputActionValue& InputActionValue);
	void OffMove(const FInputActionValue& InputActionValue);
	void OnLook(const FInputActionValue& InputActionValue);
	void OffLook(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void Grab(UMotionControllerComponent* InController);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* VRCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMotionControllerComponent* MotionControllerLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMotionControllerComponent* MotionControllerRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVRHandMeshComponent* LeftHand;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVRHandMeshComponent* RightHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHandGraph* HandGraphLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHandGraph* HandGraphRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UWidgetInteractionComponent* LeftInteraction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UWidgetInteractionComponent* RightInteraction;

private:
	class ASubMarine* RidingSubMarine;
	bool bRiding;
};
