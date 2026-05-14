#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateDistanceToTarget.generated.h"

/**
 * 行为树服务：更新“自身到目标”的距离
 *
 * 用法：
 * - 本类继承自 BTService_BlackboardBase，其 BlackboardKey 用来指定“目标对象 key”（通常填 TargetActor）
 * - DistanceKey 指定要写入的 float key（例如 DistanceToTarget）
 * - Tick 时写入距离；若无目标则写入 Max float（便于 decorator 做阈值判断）
 */
UCLASS()
class ACTGAME_API UBTService_UpdateDistanceToTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_UpdateDistanceToTarget();

	/** Blackboard：写入距离的 float key */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector DistanceKey;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

