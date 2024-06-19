#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallMachine.generated.h"

UCLASS()
class MINIGAMEPARTY_API ABallMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	ABallMachine();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SpawnBall();
	void SetDistnace(float Input);
private:
	UPROPERTY(EditAnywhere)
	USceneComponent* Scene;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* SpawnPoint;

	UPROPERTY(EditAnywhere)
	UParticleSystem* SpawnParicle;

	UPROPERTY(EditAnywhere)
	USoundBase* SpawnSound;

	TSubclassOf<class ABall> BallClass;
	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* Score;

	class UUI_Distance* Distance;
};
