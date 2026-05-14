#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RangedAttack.generated.h"

/**
 * 行为树任务：远程攻击
 *
 * 作用：
 * - 触发敌人远程攻击入口（DoRangedAttack，通常由蓝图覆写实现动画/投射物）
 * - 可选“即时命中”模式：在 Task 内根据距离直接对目标扣血（用于原型验证或无投射物实现时）
 */
UCLASS()
class ACTGAME_API UBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RangedAttack();

	/** Blackboard：目标 Actor（通常是 TargetActor） */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetActorKey;

	/** 是否由 Task 直接造成伤害（true=即时命中；false=仅触发 DoRangedAttack） */
	UPROPERTY(EditAnywhere, Category = "Attack")
	bool bInstantHit = false;

	/** 即时命中伤害（仅在 bInstantHit=true 时生效） */
	UPROPERTY(EditAnywhere, Category = "Attack", meta = (EditCondition = "bInstantHit"))
	float Damage = 8.0f;

	/** 最大有效距离（即时命中时用于距离判定；非即时命中也可作为行为树 decorator 的参考） */
	UPROPERTY(EditAnywhere, Category = "Attack")
	float MaxRange = 800.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

