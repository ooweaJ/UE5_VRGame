#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Actor/Player/HandGraph.h"
#include "Components/WidgetInteractionComponent.h"
#include "MenuPawn.generated.h"

UCLASS()
class MINIGAMEPARTY_API AMenuPawn : public APawn
{
	friend class UHandGraph;
	GENERATED_BODY()
public:
	AMenuPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnRightIndexTriggered(const FInputActionValue& InputActionValue);
	void OnRightIndexCompleted(const FInputActionValue& InputActionValue);

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UWidgetInteractionComponent* RightInteraction;

	UPROPERTY(EditAnywhere)
	UMotionControllerComponent* MotionControllerRight;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* VRCamera;

};
