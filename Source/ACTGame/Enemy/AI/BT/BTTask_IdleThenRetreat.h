#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IdleThenRetreat.generated.h"

UCLASS()
class ACTGAME_API UBTTask_IdleThenRetreat : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_IdleThenRetreat();

	UPROPERTY(EditAnywhere, Category = "Retreat")
	float IdleSeconds = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Retreat")
	float RetreatDistance = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Retreat")
	float AcceptanceRadius = 30.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
};

