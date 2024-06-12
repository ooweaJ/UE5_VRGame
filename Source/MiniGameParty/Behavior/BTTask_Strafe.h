#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Strafe.generated.h"

UCLASS()
class MINIGAMEPARTY_API UBTTask_Strafe : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Strafe();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
