#include "Enemy/AI/EnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"

AEnemyAIController::AEnemyAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*PerceptionComponent);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 2500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
	SightConfig->SetMaxAge(2.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::HandlePerceptionUpdated);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CachedEnemy = Cast<AEnemyBaseCharacter>(InPawn);
	if (CachedEnemy.IsValid())
	{
		CachedEnemy->OnStunChanged.AddDynamic(this, &AEnemyAIController::HandleStunChanged);
	}

	if (DefaultBehaviorTree)
	{
		RunBehaviorTree(DefaultBehaviorTree);
	}
}

void AEnemyAIController::OnUnPossess()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ResumeLogicTimerHandle);
	}

	if (CachedEnemy.IsValid())
	{
		CachedEnemy->OnStunChanged.RemoveDynamic(this, &AEnemyAIController::HandleStunChanged);
		CachedEnemy.Reset();
	}

	Super::OnUnPossess();
}

void AEnemyAIController::HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
	{
		return;
	}

	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB)
	{
		return;
	}

	for (AActor* Actor : UpdatedActors)
	{
		if (Actor != PlayerPawn)
		{
			continue;
		}

		FActorPerceptionBlueprintInfo Info;
		PerceptionComponent->GetActorsPerception(Actor, Info);

		bool bSensed = false;
		FVector LastLocation = Actor->GetActorLocation();
		for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
		{
			if (Stimulus.Type == SightConfig->GetSenseID())
			{
				bSensed = Stimulus.WasSuccessfullySensed();
				LastLocation = Stimulus.StimulusLocation;
				break;
			}
		}

		if (bSensed)
		{
			BB->SetValueAsObject(TargetActorKeyName, Actor);
			BB->SetValueAsVector(LastKnownLocationKeyName, Actor->GetActorLocation());
		}
		else
		{
			BB->ClearValue(TargetActorKeyName);
			BB->SetValueAsVector(LastKnownLocationKeyName, LastLocation);
		}
	}
}

void AEnemyAIController::HandleStunChanged(bool bNewStunned)
{
	if (!BrainComponent)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ResumeLogicTimerHandle);
	}

	if (bNewStunned)
	{
		StopMovement();
		BrainComponent->PauseLogic(TEXT("Stunned"));
	}
	else
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(ResumeLogicTimerHandle, this, &AEnemyAIController::ResumeLogicAfterStun, PostStunResumeDelay, false);
		}
	}
}

void AEnemyAIController::ResumeLogicAfterStun()
{
	if (BrainComponent)
	{
		BrainComponent->ResumeLogic(TEXT("StunnedEnded"));
	}
}

