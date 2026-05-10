#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DashLastKnownDirection.generated.h"

/**
 * 行为树任务：朝“最后一次感知方向”冲刺/冲锋
 *
 * 设计：
 * - 从黑板读取 LastKnownLocation，计算水平面方向并强制转向
 * - 调用敌人 DoDash（用于蓝图播放冲刺动画/特效）
 * - 使用 LaunchCharacter 推进，持续 DashDuration 后停止移动
 */
UCLASS()
class ACTGAME_API UBTTask_DashLastKnownDirection : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_DashLastKnownDirection();

	/** Blackboard：最后一次感知到目标的位置（通常是 LastKnownLocation） */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector LastKnownLocationKey;

	/** 冲刺速度（LaunchCharacter 的速度向量大小） */
	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashSpeed = 2800.0f;

	/** 冲刺持续时间（秒），到点后 StopMovementImmediately */
	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashDuration = 0.25f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
};

