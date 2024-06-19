#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UI_Distance.generated.h"

UCLASS()
class MINIGAMEPARTY_API UUI_Distance : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDistance(float InDistance);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Distance;
};
