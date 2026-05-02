#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DashLastKnownDirection.generated.h"

UCLASS()
class ACTGAME_API UBTTask_DashLastKnownDirection : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_DashLastKnownDirection();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector LastKnownLocationKey;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashSpeed = 2800.0f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashDuration = 0.25f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
};

