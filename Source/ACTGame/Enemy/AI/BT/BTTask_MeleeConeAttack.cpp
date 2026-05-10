#include "Enemy/AI/BT/BTTask_MeleeConeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "Player/Base/CharacterBase.h"

UBTTask_MeleeConeAttack::UBTTask_MeleeConeAttack()
{
	NodeName = TEXT("Melee Cone Attack");
}

EBTNodeResult::Type UBTTask_MeleeConeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* SelfPawn = AICon ? AICon->GetPawn() : nullptr;
	if (!SelfPawn)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = BB ? Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName)) : nullptr;

	// 先触发敌人侧的近战攻击入口（通常由蓝图覆写实现播放蒙太奇/特效等）
	if (AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(SelfPawn))
	{
		Enemy->DoMeleeAttack();
	}

	// 没有目标也算成功结束（避免卡死行为树）
	if (!TargetActor)
	{
		return EBTNodeResult::Succeeded;
	}

	// 1) 距离判定
	const FVector ToTarget = TargetActor->GetActorLocation() - SelfPawn->GetActorLocation();
	const float Dist = ToTarget.Size();
	if (Dist > ConeRange)
	{
		return EBTNodeResult::Succeeded;
	}

	// 2) 扇形夹角判定（仅用水平面 2D 方向）
	const FVector Forward2D = SelfPawn->GetActorForwardVector().GetSafeNormal2D();
	const FVector ToTarget2D = ToTarget.GetSafeNormal2D();
	const float CosHalfAngle = FMath::Cos(FMath::DegreesToRadians(ConeAngleDegrees * 0.5f));
	const float Dot = FVector::DotProduct(Forward2D, ToTarget2D);
	if (Dot < CosHalfAngle)
	{
		return EBTNodeResult::Succeeded;
	}

	if (ACharacterBase* TargetCharacter = Cast<ACharacterBase>(TargetActor))
	{
		TargetCharacter->ReceiveHit(Damage);
	}

	return EBTNodeResult::Succeeded;
}

