#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SubMarineMovementComponent.h"
#include "SubMarine.generated.h"

UCLASS()
class MINIGAMEPARTY_API ASubMarine : public APawn
{
	GENERATED_BODY()

public:
	ASubMarine();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void OnAuto();
	void OnRiding();
	void InputThrottle(float Throttle);
	void InputUPDown(float Value);
	void CalculateSteering(float InputValue);
	void ZEngineOff();
	void BulletFire();

	UFUNCTION()
	void OnGearBtnClicked();
	UFUNCTION()
	void OnGear2BtnClicked();
	UFUNCTION()
	void OnGear3BtnClicked();
	UFUNCTION()
	void OnRidingBtnClicked();
	UFUNCTION()
	void OnAutoBtnClicked();

	UFUNCTION()
	void OnSteeringStart();

	UFUNCTION()
	void OnSteeringStop();

	UFUNCTION()
	void OnFire();
private:
	void ChangeGear(ESubmarineGear InputGear);
	void AddYaw();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	USubMarineMovementComponent* SubMarineMovementComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SubMarine;

	UPROPERTY(EditAnywhere)
	USceneComponent* MuzzleOffSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SteeringWheel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Muzzle;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	float GravityScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
	float SteeringSensitivity = 0.5f;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* Menu;

	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	UPROPERTY(EditAnywhere)
	float MaxForce = 10;

	UPROPERTY(EditAnywhere)
	float FireRate = 0.5f;

	class UUI_SubMarine* SubMarineMenu;
private:
	class AVRCharacter* RidingCharacter;
	TSubclassOf<class ABullet> BulletClass;
	FVector InitialLeftControllerPosition;
	FVector InitialRightControllerPosition;

	float EnginePower = 100.f;

	bool bIsSteering;
	bool bAuto;
	bool bFire;
};
