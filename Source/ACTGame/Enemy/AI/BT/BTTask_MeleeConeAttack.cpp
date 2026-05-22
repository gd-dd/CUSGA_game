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
		UE_LOG(LogTemp, Warning, TEXT("[EnemyBT][Melee] ExecuteTask failed: SelfPawn is null"));
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = BB ? Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName)) : nullptr;
	UE_LOG(LogTemp, Warning, TEXT("[EnemyBT][Melee] Start: Self=%s, Target=%s, Key=%s"),
		*GetNameSafe(SelfPawn),
		*GetNameSafe(TargetActor),
		*TargetActorKey.SelectedKeyName.ToString());

	// 先触发敌人侧的近战攻击入口（通常由蓝图覆写实现播放蒙太奇/特效等）
	if (AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(SelfPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyBT][Melee] Calling DoMeleeAttack on %s"), *GetNameSafe(Enemy));
		Enemy->DoMeleeAttack();
	}

	// 没有目标也算成功结束（避免卡死行为树）
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyBT][Melee] No target actor, task ends without damage"));
		return EBTNodeResult::Succeeded;
	}

	// 1) 距离判定
	const FVector ToTarget = TargetActor->GetActorLocation() - SelfPawn->GetActorLocation();
	const float Dist = ToTarget.Size();
	if (Dist > ConeRange)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyBT][Melee] Target out of range: Dist=%.2f, ConeRange=%.2f"), Dist, ConeRange);
		return EBTNodeResult::Succeeded;
	}

	// 2) 扇形夹角判定（仅用水平面 2D 方向）
	const FVector Forward2D = SelfPawn->GetActorForwardVector().GetSafeNormal2D();
	const FVector ToTarget2D = ToTarget.GetSafeNormal2D();
	const float CosHalfAngle = FMath::Cos(FMath::DegreesToRadians(ConeAngleDegrees * 0.5f));
	const float Dot = FVector::DotProduct(Forward2D, ToTarget2D);
	if (Dot < CosHalfAngle)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyBT][Melee] Target outside cone: Dot=%.3f, Required=%.3f, Angle=%.2f"),
			Dot, CosHalfAngle, ConeAngleDegrees);
		return EBTNodeResult::Succeeded;
	}

	if (ACharacterBase* TargetCharacter = Cast<ACharacterBase>(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyBT][Melee] Hit success: Target=%s, Damage=%.2f"), *GetNameSafe(TargetCharacter), Damage);
		TargetCharacter->ReceiveHit(Damage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyBT][Melee] Target is not ACharacterBase: %s"), *GetNameSafe(TargetActor));
	}

	return EBTNodeResult::Succeeded;
}

