#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PatrolRandomPoint.generated.h"

UCLASS()
class ACTGAME_API UBTTask_PatrolRandomPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PatrolRandomPoint();

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float PatrolRadius = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float AcceptanceRadius = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float MoveTimeoutSeconds = 8.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
};

