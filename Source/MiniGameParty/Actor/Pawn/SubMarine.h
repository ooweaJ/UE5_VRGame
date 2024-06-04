#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class USubMarineMovementComponent* SubMarineMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	float GravityScale = 1.f;
};
