#include "Enemy/AI/BT/BTService_UpdateDistanceToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateDistanceToTarget::UBTService_UpdateDistanceToTarget()
{
	NodeName = TEXT("Update Distance To Target");
	Interval = 0.2f;
	RandomDeviation = 0.05f;
}

void UBTService_UpdateDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return;
	}

	UObject* TargetObj = BB->GetValueAsObject(BlackboardKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetObj);

	APawn* SelfPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!SelfPawn)
	{
		return;
	}

	const float Distance = TargetActor ? FVector::Dist(SelfPawn->GetActorLocation(), TargetActor->GetActorLocation()) : TNumericLimits<float>::Max();
	if (DistanceKey.SelectedKeyType)
	{
		BB->SetValueAsFloat(DistanceKey.SelectedKeyName, Distance);
	}
}

