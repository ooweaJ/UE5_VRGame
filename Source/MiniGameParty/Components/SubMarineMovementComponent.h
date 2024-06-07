// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "SubMarineMovementComponent.generated.h"


UENUM(BlueprintType)
enum class ESubmarineGear : uint8
{
    Gear1 UMETA(DisplayName = "Gear 1"),
    Gear2 UMETA(DisplayName = "Gear 2"),
    Gear3 UMETA(DisplayName = "Gear 3"),
    Max
};

UCLASS()
class MINIGAMEPARTY_API USubMarineMovementComponent : public UFloatingPawnMovement
{
	GENERATED_BODY()

public:
    USubMarineMovementComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual float GetGravityZ() const override;

public:
    FORCEINLINE ESubmarineGear GetCurrentGear() { return CurrentGear; }

    void SetPawnOwner(class ASubMarine* Pawn);
    void UpdateMaxSpeed(ESubmarineGear InputGear);
    void InputVector(FVector Input);

private:
    UFUNCTION()
    void OffBlockingDelay();

private:
    void CollisionDetection(float DeltaTime);
    void BlockingDelayInputTime(float DelayTime);

private:
    ESubmarineGear CurrentGear;

    float Dragcoefficient = 0.1f; // Res(MaxForce) =  Velocity ^ 2 * coefficient -> coefficient = Res / Speed ^ 2

    // SubMarine
    class USphereComponent* Sphere;
    class ASubMarine* OwnerPawn;
    float MaxSpeedGear[(int32)ESubmarineGear::Max] = { 500, 1000, 1500 };
    float InterpSpeed = 0.1f;
    bool bBlocking;
    bool bInput;
};
