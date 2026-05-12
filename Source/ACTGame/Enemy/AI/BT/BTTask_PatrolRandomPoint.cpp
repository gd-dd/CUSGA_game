#include "Enemy/AI/BT/BTTask_PatrolRandomPoint.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

namespace
{
	struct FPatrolTaskMemory
	{
		float StartTime = 0.0f;
		bool bMoveIssued = false;
		FVector Dest = FVector::ZeroVector;
	};
}

UBTTask_PatrolRandomPoint::UBTTask_PatrolRandomPoint()
{
	NodeName = TEXT("Patrol Random Point");
	bNotifyTick = true;
}

uint16 UBTTask_PatrolRandomPoint::GetInstanceMemorySize() const
{
	return sizeof(FPatrolTaskMemory);
}

EBTNodeResult::Type UBTTask_PatrolRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FPatrolTaskMemory* Mem = (FPatrolTaskMemory*)NodeMemory;
	Mem->bMoveIssued = false;
	Mem->Dest = FVector::ZeroVector;

	AAIController* AICon = OwnerComp.GetAIOwner();
	ACharacter* SelfChar = AICon ? Cast<ACharacter>(AICon->GetPawn()) : nullptr;
	if (!SelfChar || !SelfChar->GetWorld())
	{
		return EBTNodeResult::Failed;
	}

	Mem->StartTime = SelfChar->GetWorld()->GetTimeSeconds();

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(SelfChar->GetWorld());
	if (!NavSys)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation Out;
	const bool bFound = NavSys->GetRandomReachablePointInRadius(SelfChar->GetActorLocation(), PatrolRadius, Out);
	if (!bFound)
	{
		return EBTNodeResult::Failed;
	}

	Mem->Dest = Out.Location;
	AICon->StopMovement();
	AICon->MoveToLocation(Mem->Dest, AcceptanceRadius, true, true, false, true, nullptr, true);
	Mem->bMoveIssued = true;
	return EBTNodeResult::InProgress;
}

void UBTTask_PatrolRandomPoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FPatrolTaskMemory* Mem = (FPatrolTaskMemory*)NodeMemory;
	AAIController* AICon = OwnerComp.GetAIOwner();
	ACharacter* SelfChar = AICon ? Cast<ACharacter>(AICon->GetPawn()) : nullptr;
	if (!SelfChar || !SelfChar->GetWorld() || !AICon || !Mem->bMoveIssued)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const float Now = SelfChar->GetWorld()->GetTimeSeconds();
	if (MoveTimeoutSeconds > 0.0f && Now > Mem->StartTime + MoveTimeoutSeconds)
	{
		AICon->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const UPathFollowingComponent* PathComp = AICon->GetPathFollowingComponent();
	const EPathFollowingStatus::Type Status = PathComp ? PathComp->GetStatus() : EPathFollowingStatus::Idle;
	if (Status == EPathFollowingStatus::Moving || Status == EPathFollowingStatus::Waiting)
	{
		return;
	}

	const float Dist = FVector::Dist(SelfChar->GetActorLocation(), Mem->Dest);
	FinishLatentTask(OwnerComp, Dist <= AcceptanceRadius ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

