#include "Behavior/BTTask_Strafe.h"
#include "Actor/Controller/SharkAIController.h"
#include "Actor/Pawn/Enemy/Shark.h"

UBTTask_Strafe::UBTTask_Strafe()
{
	NodeName = "Strafe";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASharkAIController* controller = Cast<ASharkAIController>(OwnerComp.GetOwner());
	AShark* aiPawn = Cast<AShark>(controller->GetPawn());

	aiPawn->OnStrafe();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Strafe::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ASharkAIController* controller = Cast<ASharkAIController>(OwnerComp.GetOwner());
	AShark* aiPawn = Cast<AShark>(controller->GetPawn());

	if(!aiPawn->IsStrafe())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
