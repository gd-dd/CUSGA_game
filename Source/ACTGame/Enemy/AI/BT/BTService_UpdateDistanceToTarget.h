#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateDistanceToTarget.generated.h"

UCLASS()
class ACTGAME_API UBTService_UpdateDistanceToTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_UpdateDistanceToTarget();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector DistanceKey;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

