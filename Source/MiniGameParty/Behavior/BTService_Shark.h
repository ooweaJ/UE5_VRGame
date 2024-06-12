#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Shark.generated.h"


UCLASS()
class MINIGAMEPARTY_API UBTService_Shark : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_Shark();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
