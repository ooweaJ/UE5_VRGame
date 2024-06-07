#pragma once

#include "CoreMinimal.h"

struct MINIGAMEPARTY_API FCollisionPresetNameTable
{
	static inline FName Player = TEXT("Player");
	static inline FName Enemy = TEXT("Enemy");
	static inline FName Static = TEXT("OnlyStatic");
};

struct MINIGAMEPARTY_API FCollisionChannel
{
	static inline ECollisionChannel WorldUI = ECollisionChannel::ECC_GameTraceChannel2;
};