#include "Components/GrabComponent.h"

UGrabComponent::UGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UGrabComponent::Grab(FTransform TargetTransform)
{
	AActor* Owner = GetOwner();
	Owner->SetActorTransform(TargetTransform);
	Owner->AddActorLocalRotation(FRotator(-90,0,0));
	Owner->AddActorLocalOffset(-GetRelativeLocation());
}

void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

