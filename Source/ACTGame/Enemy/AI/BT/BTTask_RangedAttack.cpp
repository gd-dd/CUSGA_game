#include "Enemy/AI/BT/BTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "Player/Base/CharacterBase.h"

UBTTask_RangedAttack::UBTTask_RangedAttack()
{
	NodeName = TEXT("Ranged Attack");
}

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* SelfPawn = AICon ? AICon->GetPawn() : nullptr;
	if (!SelfPawn)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = BB ? Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName)) : nullptr;

	// 先触发敌人侧的远程攻击入口（通常由蓝图覆写实现播放动画/生成投射物等）
	if (AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(SelfPawn))
	{
		Enemy->DoRangedAttack();
	}

	// 非即时命中：只负责触发攻击表现，伤害交由投射物/动画通知等处理
	if (!bInstantHit || !TargetActor)
	{
		return EBTNodeResult::Succeeded;
	}

	// 即时命中：距离满足时直接对目标扣血（用于原型或无投射物实现的情况）
	const float Dist = FVector::Dist(SelfPawn->GetActorLocation(), TargetActor->GetActorLocation());
	if (Dist > MaxRange)
	{
		return EBTNodeResult::Succeeded;
	}

	if (ACharacterBase* TargetCharacter = Cast<ACharacterBase>(TargetActor))
	{
		TargetCharacter->ReceiveHit(Damage);
	}

	return EBTNodeResult::Succeeded;
}

