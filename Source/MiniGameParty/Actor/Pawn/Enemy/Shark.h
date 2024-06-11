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
	
public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Head;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere)
	UFloatingPawnMovement* FloatingPawnMovement;

	class UCurveFloat* Curve;

	FTimeline Timeline;
	FOnTimelineFloat TimelineFloat;
	FOnTimelineEvent TimelineFinished;

	bool bFlipFlop = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	AActor* CenterActor;  // 상어가 중심으로 삼을 Actor

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CircleRadius = 600.0f;  // 원의 반지름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 50.0f;  // 회전 속도 (도/초)

	float CurrentAngle;  // 현재 각도
};
