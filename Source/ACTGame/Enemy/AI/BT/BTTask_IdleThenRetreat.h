#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IdleThenRetreat.generated.h"

/**
 * 行为树任务：原地停顿一段时间后撤退
 *
 * 适用场景：
 * - 远程怪在玩家贴脸时，先 idle（给出“想后撤”的反应时间），再向后找点并 MoveTo
 */
UCLASS()
class ACTGAME_API UBTTask_IdleThenRetreat : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_IdleThenRetreat();

	/** 原地停顿时间（秒） */
	UPROPERTY(EditAnywhere, Category = "Retreat")
	float IdleSeconds = 1.0f;

	/** 期望后撤距离（沿角色前方向的反方向） */
	UPROPERTY(EditAnywhere, Category = "Retreat")
	float RetreatDistance = 400.0f;

	/** MoveTo 的接受半径 */
	UPROPERTY(EditAnywhere, Category = "Retreat")
	float AcceptanceRadius = 30.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
};

