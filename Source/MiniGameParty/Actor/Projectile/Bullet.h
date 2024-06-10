#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class MINIGAMEPARTY_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* ImpactParticle;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* ThrowParticle;

	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* Projectile;

	UPROPERTY(VisibleDefaultsOnly)
	class USceneComponent* Scene;
private:
	UPROPERTY(EditAnywhere)
	float ProjectileSpeed = 1500.f;

	UPROPERTY(EditAnywhere)
	float MaxDistance = 500.f;

};
