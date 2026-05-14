#include "Enemy/AI/BT/BTTask_IdleThenRetreat.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

namespace
{
	struct FIdleRetreatTaskMemory
	{
		// 任务开始时间（世界时间秒），用于计算 idle 截止点
		float StartTime = 0.0f;
		// 是否已下发 MoveTo，避免重复调用
		bool bMoveIssued = false;
		// 计算出的撤退目标点（已导航投射）
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

	// 期望撤退点：沿“前方反方向”退 RetreatDistance
	const FVector Desired = SelfChar->GetActorLocation() - SelfChar->GetActorForwardVector().GetSafeNormal2D() * RetreatDistance;
	FVector Projected = Desired;
	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(SelfChar->GetWorld()))
	{
		FNavLocation Out;
		if (NavSys->ProjectPointToNavigation(Desired, Out))
		{
			// 投射到可行走区域，避免移动到不可达点
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
		// idle 时间未到：保持原地等待
		if (Now < Mem->StartTime + IdleSeconds)
		{
			return;
		}

		AICon->StopMovement();
		// idle 结束后再下发 MoveTo，后撤到计算出的目标点
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

	// MoveTo 结束：用距离做最终成功/失败判定
	const float Dist = FVector::Dist(SelfChar->GetActorLocation(), Mem->RetreatLocation);
	FinishLatentTask(OwnerComp, Dist <= AcceptanceRadius ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

