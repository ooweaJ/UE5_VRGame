#include "Behavior/BTTask_Attack.h"
#include "Actor/Controller/SharkAIController.h"
#include "Actor/Pawn/Enemy/Shark.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ASharkAIController* controller = Cast<ASharkAIController>(OwnerComp.GetOwner());
	AShark* aiPawn = Cast<AShark>(controller->GetPawn());

	if (!aiPawn->IsAttack())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
