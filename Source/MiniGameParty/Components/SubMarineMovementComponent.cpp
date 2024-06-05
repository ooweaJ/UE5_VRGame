#include "Components/SubMarineMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Actor/Pawn/SubMarine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

USubMarineMovementComponent::USubMarineMovementComponent()
{
}

void USubMarineMovementComponent::BeginPlay()
{
    if (OwnerPawn)
    {
        Sphere = OwnerPawn->FindComponentByClass<USphereComponent>();
    }
    UpdateMaxSpeed(ESubmarineGear::Gear1);
}

void USubMarineMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    //FVector TargetVelocity = UKismetMathLibrary::VInterpTo(Velocity, FVector::Zero(), DeltaTime, 5 * InterpSpeed);
    //Velocity = TargetVelocity;

    CollisionDetection(DeltaTime);
}

float USubMarineMovementComponent::GetGravityZ() const
{
    return Super::GetGravityZ() * OwnerPawn->GravityScale;
}

void USubMarineMovementComponent::SetPawnOwner(ASubMarine* Pawn)
{
    OwnerPawn = Pawn;
}

void USubMarineMovementComponent::OffBlockingDelay()
{
    bBlocking = false;
}

void USubMarineMovementComponent::CollisionDetection(float DeltaTime)
{
    if (!OwnerPawn || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }
   

    FVector DesiredMovementThisFrame = Velocity * DeltaTime;

    if (!DesiredMovementThisFrame.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

        if (Hit.IsValidBlockingHit())
        {
            FVector ImpactDirection = Hit.ImpactNormal;
            DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + ImpactDirection * 100.f, FColor::Green, false, 10.f, 0, 1.f);
            float DotProduct = FVector::DotProduct(ImpactDirection, Velocity.GetSafeNormal());

            float DelaySize = Velocity.Size() / MaxSpeedGear[(int32)ESubmarineGear::Gear3];

            if (DotProduct < -0.5)
            {
                FVector ReflectionDirection = UKismetMathLibrary::GetReflectionVector(Velocity, Hit.Normal).GetSafeNormal();
                float CanSafePowerDot = FVector::DotProduct(ImpactDirection, ReflectionDirection);
                
                if (CanSafePowerDot < 0)
                {
                    Velocity = UKismetMathLibrary::GetReflectionVector(Velocity, Hit.Normal) * (-CanSafePowerDot);
                    DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + Velocity, FColor::Black, false, 10.f, 0, 1.f);
                }
                else
                {
                    Velocity = UKismetMathLibrary::GetReflectionVector(Velocity, Hit.Normal) * (1.f + DotProduct);
                    DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + Velocity, FColor::Blue, false, 10.f, 0, 1.f);
                }
            }
            else if(DotProduct > -0.5)
            {
                DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + UKismetMathLibrary::GetReflectionVector(Velocity, Hit.Normal) * 100.f, FColor::Red, false, 10.f, 0, 1.f);
                Velocity = UKismetMathLibrary::GetReflectionVector(Velocity, Hit.Normal) * ( 1.f + DotProduct);
            }

            BlockingDelayInputTime(DelaySize);
        }
    }
}

void USubMarineMovementComponent::BlockingDelayInputTime(float DelayTime)
{
    bBlocking = true;
    UKismetSystemLibrary::K2_SetTimer(this, "OffBlockingDelay", DelayTime, false);
}


void USubMarineMovementComponent::UpdateMaxSpeed(ESubmarineGear InputGear)
{
    switch (InputGear)
    {
    case ESubmarineGear::Gear1:
        CurrentGear = InputGear;
        MaxSpeed = MaxSpeedGear[(int32)ESubmarineGear::Gear1];
        break;
    case ESubmarineGear::Gear2:
        CurrentGear = InputGear;
        MaxSpeed = MaxSpeedGear[(int32)ESubmarineGear::Gear2];
        break;
    case ESubmarineGear::Gear3:
        CurrentGear = InputGear;
        MaxSpeed = MaxSpeedGear[(int32)ESubmarineGear::Gear3];
        break;
    }
}

void USubMarineMovementComponent::InputVector(FVector Input)
{

    Velocity += Input;

    float CurrentSpeed = Velocity.Size();

    if (CurrentSpeed > MaxSpeed)
    {
        Velocity = Velocity.GetSafeNormal() * MaxSpeed;
    }
}

