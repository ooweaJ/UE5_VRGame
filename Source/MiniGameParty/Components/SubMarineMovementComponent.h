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
    void SetPawnOwner(class ASubMarine* Pawn);
    void UpdateMaxSpeed(ESubmarineGear CurrentGear);
    
private:
    void OnGravity(float DeltaTime);

private:

    // SubMarine
    class USphereComponent* Sphere;
    class ASubMarine* OwnerPawn;
    float MaxSpeedGear[(int32)ESubmarineGear::Max] = { 100, 200, 300 };
    float InterpSpeed = 0.1f;
};
