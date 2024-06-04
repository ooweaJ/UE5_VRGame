// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "SubMarineMovementComponent.generated.h"

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


private:
    UPROPERTY(EditDefaultsOnly)
    class USphereComponent* Sphere;

private:
    class ASubMarine* OwnerPawn;
};
