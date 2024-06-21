#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UI_SubMarine.generated.h"

UCLASS()
class MINIGAMEPARTY_API UUI_SubMarine : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void SelectGear(int32 SelectIndex);
	void SelectAuto(bool IsAuto);
	void OnRiding();
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Gear;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Gear2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Gear3;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Riding;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Auto;

	TArray<UButton*> Gears;
};
