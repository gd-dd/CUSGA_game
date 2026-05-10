#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MeleeConeAttack.generated.h"

/**
 * 行为树任务：近战扇形攻击
 *
 * 作用：
 * - 触发敌人近战攻击入口（DoMeleeAttack，通常由蓝图覆写实现动画/特效）
 * - 在 Task 内做“扇形判定”（距离 + 夹角），满足时对目标造成伤害
 */
UCLASS()
class ACTGAME_API UBTTask_MeleeConeAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MeleeConeAttack();

	/** Blackboard：目标 Actor（通常是 TargetActor） */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetActorKey;

	/** 造成伤害值 */
	UPROPERTY(EditAnywhere, Category = "Attack")
	float Damage = 10.0f;

	/** 扇形最大判定距离 */
	UPROPERTY(EditAnywhere, Category = "Attack")
	float ConeRange = 300.0f;

	/** 扇形角度（度） */
	UPROPERTY(EditAnywhere, Category = "Attack")
	float ConeAngleDegrees = 120.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

