// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Shark.generated.h"

UCLASS()
class MINIGAMEPARTY_API AShark : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShark();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void SharkMove(float Infloat);

	UFUNCTION()
	void TimelineHandle();
	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void Targetbutt();

	FORCEINLINE bool IsStrafe() { return bStrafe; }
	FORCEINLINE bool IsAttack() { return bAttack; }

	void EndAttack();
	void TargetStrafe(float DeltaTime);
	void OnStrafe();
	void Return(float DeltaTime);
public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Head;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UFloatingPawnMovement* FloatingPawnMovement;

	class UCurveFloat* Curve;

	FTimeline Timeline;
	FOnTimelineFloat TimelineFloat;
	FOnTimelineEvent TimelineFinished;

	bool bFlipFlop = true;
	bool bAttack;
	bool bStrafe;
	
	AActor* CenterActor; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CircleRadius = 600.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 50.0f; 

	float CurrentAngle;

	TSubclassOf<UCameraShakeBase> CameraShake;
	FVector TargetDirection;

private:
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* ImpactParticle;
	FVector InitialLocation;
	FVector ReturnLocation;
};
