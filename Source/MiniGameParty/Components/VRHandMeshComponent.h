#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "VRHandMeshComponent.generated.h"

UCLASS()
class MINIGAMEPARTY_API UVRHandMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	bool bMirror;

};
