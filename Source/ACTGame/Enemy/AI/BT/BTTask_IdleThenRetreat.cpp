#include "Enemy/AI/BT/BTTask_IdleThenRetreat.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

namespace
{
	struct FIdleRetreatTaskMemory
	{
		float StartTime = 0.0f;
		bool bMoveIssued = false;
		FVector RetreatLocation = FVector::ZeroVector;
	};
}

UBTTask_IdleThenRetreat::UBTTask_IdleThenRetreat()
{
	NodeName = TEXT("Idle Then Retreat");
	bNotifyTick = true;
}

uint16 UBTTask_IdleThenRetreat::GetInstanceMemorySize() const
{
	return sizeof(FIdleRetreatTaskMemory);
}

EBTNodeResult::Type UBTTask_IdleThenRetreat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FIdleRetreatTaskMemory* Mem = (FIdleRetreatTaskMemory*)NodeMemory;
	Mem->bMoveIssued = false;

	AAIController* AICon = OwnerComp.GetAIOwner();
	ACharacter* SelfChar = AICon ? Cast<ACharacter>(AICon->GetPawn()) : nullptr;
	if (!SelfChar || !SelfChar->GetWorld())
	{
		return EBTNodeResult::Failed;
	}

	Mem->StartTime = SelfChar->GetWorld()->GetTimeSeconds();

	const FVector Desired = SelfChar->GetActorLocation() - SelfChar->GetActorForwardVector().GetSafeNormal2D() * RetreatDistance;
	FVector Projected = Desired;
	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(SelfChar->GetWorld()))
	{
		FNavLocation Out;
		if (NavSys->ProjectPointToNavigation(Desired, Out))
		{
			Projected = Out.Location;
		}
	}

	Mem->RetreatLocation = Projected;
	return EBTNodeResult::InProgress;
}

void UBTTask_IdleThenRetreat::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FIdleRetreatTaskMemory* Mem = (FIdleRetreatTaskMemory*)NodeMemory;
	AAIController* AICon = OwnerComp.GetAIOwner();
	ACharacter* SelfChar = AICon ? Cast<ACharacter>(AICon->GetPawn()) : nullptr;
	if (!SelfChar || !SelfChar->GetWorld())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const float Now = SelfChar->GetWorld()->GetTimeSeconds();
	if (!Mem->bMoveIssued)
	{
		if (Now < Mem->StartTime + IdleSeconds)
		{
			return;
		}

		AICon->StopMovement();
		AICon->MoveToLocation(Mem->RetreatLocation, AcceptanceRadius, true, true, false, true, nullptr, true);
		Mem->bMoveIssued = true;
		return;
	}

	const UPathFollowingComponent* PathComp = AICon->GetPathFollowingComponent();
	const EPathFollowingStatus::Type Status = PathComp ? PathComp->GetStatus() : EPathFollowingStatus::Idle;
	if (Status == EPathFollowingStatus::Moving || Status == EPathFollowingStatus::Waiting)
	{
		return;
	}

	const float Dist = FVector::Dist(SelfChar->GetActorLocation(), Mem->RetreatLocation);
	FinishLatentTask(OwnerComp, Dist <= AcceptanceRadius ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

