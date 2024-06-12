#include "Actor/Controller/SharkAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Actor/Pawn/SubMarine.h"
#include "Actor/Pawn/Enemy/Shark.h"

ASharkAIController::ASharkAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackBoard");
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 3000.f;
	Sight->LoseSightRadius = 3000.f;
	Sight->PeripheralVisionAngleDegrees = 360.0f;
	Sight->SetMaxAge(2.f);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(Sight->GetSenseImplementation());
	SetGenericTeamId(FGenericTeamId(1));

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Script/AIModule.BlackboardData'/Game/_Dev/BB_Shark.BB_Shark'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Script/AIModule.BehaviorTree'/Game/_Dev/BT_Shark.BT_Shark'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

ETeamAttitude::Type ASharkAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (Cast<ASubMarine>(OtherPawn))
			return ETeamAttitude::Hostile;
		if (auto const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			if (TeamAgent->GetGenericTeamId() == FGenericTeamId(1))
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;
			}
		}
	}
	return ETeamAttitude::Neutral;
}

AActor* ASharkAIController::GetTarget()
{
	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject("Target"));
	return Target;
}

void ASharkAIController::SetAttack(bool Attack)
{
	Blackboard->SetValueAsBool("Attack", Attack);
}

void ASharkAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerPawn = Cast<AShark>(InPawn);
	UBlackboardComponent* BlackboardPtr = Blackboard;
	UseBlackboard(BBAsset, BlackboardPtr);
	Perception->OnPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdated);
	RunBehaviorTree(BTAsset);
}

void ASharkAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	ASubMarine* SubMarine = nullptr;

	for (AActor* actor : actors)
	{
		SubMarine = Cast<ASubMarine>(actor);

		if (!!SubMarine)
			break;
	}

	Blackboard->SetValueAsObject("Target", SubMarine);
	OwnerPawn->CenterActor = SubMarine;
}
