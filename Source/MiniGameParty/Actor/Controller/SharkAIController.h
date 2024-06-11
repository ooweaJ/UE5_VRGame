// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SharkAIController.generated.h"

UCLASS()
class MINIGAMEPARTY_API ASharkAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASharkAIController();

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
protected:

	virtual void OnPossess(APawn* InPawn) override;

private:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UPROPERTY(VisibleDefaultsOnly)
	class UAIPerceptionComponent* Perception;
	class UAISenseConfig_Sight* Sight;

	class AShark* OwnerPawn;
};
