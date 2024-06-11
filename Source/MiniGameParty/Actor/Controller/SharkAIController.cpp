#include "Actor/Controller/SharkAIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Actor/Pawn/SubMarine.h"
#include "Actor/Pawn/Enemy/Shark.h"

ASharkAIController::ASharkAIController()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 5000.f;
	Sight->LoseSightRadius = 5000.f;
	Sight->PeripheralVisionAngleDegrees = 180.0f;
	Sight->SetMaxAge(2.f);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(Sight->GetSenseImplementation());
	SetGenericTeamId(FGenericTeamId(1));
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

void ASharkAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerPawn = Cast<AShark>(InPawn);
	Perception->OnPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdated);
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

	OwnerPawn->CenterActor = SubMarine;
}
