#include "Components/SubMarineMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Actor/Pawn/SubMarine.h"

USubMarineMovementComponent::USubMarineMovementComponent()
{
}

void USubMarineMovementComponent::BeginPlay()
{
    if (OwnerPawn)
    {
        Sphere = OwnerPawn->FindComponentByClass<USphereComponent>();
    }

    Velocity = FVector::ZeroVector;
}

void USubMarineMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }

    // Apply gravity
    FVector GravityForce = FVector(0.0f, 0.0f, GetGravityZ()) * DeltaTime;

    // Update velocity with gravity
    Velocity = Velocity + GravityForce;

    // Calculate movement this frame
    FVector DesiredMovementThisFrame = Velocity * DeltaTime;

    if (!DesiredMovementThisFrame.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

        // If we bumped into something, slide along the surface
        if (Hit.IsValidBlockingHit())
        {
            SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
        }
    }
}

float USubMarineMovementComponent::GetGravityZ() const
{
    return Super::GetGravityZ() * OwnerPawn->GravityScale;
}

void USubMarineMovementComponent::SetPawnOwner(ASubMarine* Pawn)
{
    OwnerPawn = Pawn;
}
