#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MeleeConeAttack.generated.h"

UCLASS()
class ACTGAME_API UBTTask_MeleeConeAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MeleeConeAttack();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float ConeRange = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float ConeAngleDegrees = 120.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

