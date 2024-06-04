#include "Animation/VRAnimInstance.h"
#include "Components/VRHandMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#if WITH_EDITOR
#include "Animation/DebugSkelMeshComponent.h"
#include "Animation/SkeletalMeshActor.h"
#endif

void UVRAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	USkeletalMeshComponent* SkeletalMeshComponent = GetOwningComponent();
#if WITH_EDITOR
	UDebugSkelMeshComponent* DebugSkelMeshComponent = Cast<UDebugSkelMeshComponent>(SkeletalMeshComponent);
	if (DebugSkelMeshComponent) { return; }

	AActor* Actor = GetOwningActor();
	ASkeletalMeshActor* SkeletalMeshActor = Cast<ASkeletalMeshActor>(Actor);
	if (SkeletalMeshActor) { return; }
#endif

	UVRHandMeshComponent* VRHandSkeletalMeshComponent = Cast<UVRHandMeshComponent>(SkeletalMeshComponent);
	check(VRHandSkeletalMeshComponent);
	bMirror = VRHandSkeletalMeshComponent->bMirror;
}

void UVRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CurrentPoseAlphaPoint = UKismetMathLibrary::FInterpTo(CurrentPoseAlphaPoint, PoseAlphaPoint, DeltaSeconds, 13.0);
	CurrentPoseAlphaThumb = UKismetMathLibrary::FInterpTo(CurrentPoseAlphaThumb, PoseAlphaThumb, DeltaSeconds, 13.0);
}
