#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RangedAttack.generated.h"

UCLASS()
class ACTGAME_API UBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RangedAttack();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Attack")
	bool bInstantHit = false;

	UPROPERTY(EditAnywhere, Category = "Attack", meta = (EditCondition = "bInstantHit"))
	float Damage = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float MaxRange = 800.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

