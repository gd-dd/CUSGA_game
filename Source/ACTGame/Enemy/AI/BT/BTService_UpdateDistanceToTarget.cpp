#include "Enemy/AI/BT/BTService_UpdateDistanceToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateDistanceToTarget::UBTService_UpdateDistanceToTarget()
{
	NodeName = TEXT("Update Distance To Target");
	// 以较高频率刷新距离，便于行为树用阈值做快速切换
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

	// BTService_BlackboardBase::BlackboardKey 指向“目标对象”的 key（通常为 TargetActor）
	UObject* TargetObj = BB->GetValueAsObject(BlackboardKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetObj);

	APawn* SelfPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!SelfPawn)
	{
		return;
	}

	// 没有目标时写入一个极大值，便于 decorator 做范围判断（例如 <= 200 会直接失败）
	const float Distance = TargetActor ? FVector::Dist(SelfPawn->GetActorLocation(), TargetActor->GetActorLocation()) : TNumericLimits<float>::Max();
	if (DistanceKey.SelectedKeyType)
	{
		BB->SetValueAsFloat(DistanceKey.SelectedKeyName, Distance);
	}
}

