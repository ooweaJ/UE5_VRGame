#include "Behavior/BTService_Shark.h"
#include "Actor/Controller/SharkAIController.h"
#include "Actor/Pawn/Enemy/Shark.h"

UBTService_Shark::UBTService_Shark()
{
	NodeName = "Shark";
}

void UBTService_Shark::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASharkAIController* controller = Cast<ASharkAIController>(OwnerComp.GetOwner());
	AShark* aiPawn = Cast<AShark>(controller->GetPawn());
	AActor* Target = controller->GetTarget();
	float distance = aiPawn->GetDistanceTo(Target);
	
	FVector SharkLocation = aiPawn->GetActorLocation();
	FVector SharkForward = aiPawn->GetActorForwardVector();

	if (!Target) return;
	FVector TargetLocation = Target->GetActorLocation();
	FVector ToTarget = TargetLocation - SharkLocation;
	ToTarget.Normalize();

	float DotProduct = FVector::DotProduct(SharkForward, ToTarget);

	if (aiPawn->IsAttack())
	{
		if (distance >= 2000.f)
		{
			if (DotProduct < -0.7f)
			{
				aiPawn->EndAttack();
			}
		}
	}
}
