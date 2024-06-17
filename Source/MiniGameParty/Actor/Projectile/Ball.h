#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class MINIGAMEPARTY_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	ABall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void HitBat(FVector AddForce);

private:
	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* Projectile;

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* Ball;

private:
	UPROPERTY(EditAnywhere)
	float ProjectileSpeed = 1500.f;

	FRotator RollDirection;

	bool bHitted;
};
