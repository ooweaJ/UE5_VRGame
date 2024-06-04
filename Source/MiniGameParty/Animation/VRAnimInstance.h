#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VRAnimInstance.generated.h"

UCLASS()
class MINIGAMEPARTY_API UVRAnimInstance : public UAnimInstance
{
	friend class UHandGraph;
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bMirror = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PoseAlphaGrap = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PoseAlphaPoint = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentPoseAlphaPoint = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PoseAlphaIndexCurl = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PoseAlphaThumb = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentPoseAlphaThumb = 1.f;
};
