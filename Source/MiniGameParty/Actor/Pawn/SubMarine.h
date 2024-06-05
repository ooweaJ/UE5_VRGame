#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SubMarineMovementComponent.h"
#include "SubMarine.generated.h"

UCLASS()
class MINIGAMEPARTY_API ASubMarine : public APawn
{
	GENERATED_BODY()

public:
	ASubMarine();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	void OnEngine();


	UFUNCTION()
	void OnGearBtnClicked();
	UFUNCTION()
	void OnGear2BtnClicked();
	UFUNCTION()
	void OnGear3BtnClicked();
	UFUNCTION()
	void OnRidingBtnClicked();
	UFUNCTION()
	void OnAutoBtnClicked();

private:
	void ChangeGear(ESubmarineGear InputGear);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	USubMarineMovementComponent* SubMarineMovementComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SubMarine;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	float GravityScale = 1.f;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* Menu;

	class UUI_SubMarine* SubMarineMenu;

private:
	bool bAuto;
};
