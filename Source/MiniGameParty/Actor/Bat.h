// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bat.generated.h"

UCLASS()
class MINIGAMEPARTY_API ABat : public AActor
{
	GENERATED_BODY()
	
public:	
	ABat();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
private:
	UPROPERTY(VisibleDefaultsOnly)
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleDefaultsOnly)
	USkeletalMeshComponent* Mesh;

	FVector Force;
private:

	FVector PreviousLocation;
	FVector CurrentVelocity;
};
