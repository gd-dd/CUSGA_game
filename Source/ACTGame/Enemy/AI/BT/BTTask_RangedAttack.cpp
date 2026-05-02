#include "Enemy/AI/BT/BTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "Player/Base/CharacterBase.h"

UBTTask_RangedAttack::UBTTask_RangedAttack()
{
	NodeName = TEXT("Ranged Attack");
}

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* SelfPawn = AICon ? AICon->GetPawn() : nullptr;
	if (!SelfPawn)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = BB ? Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName)) : nullptr;

	if (AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(SelfPawn))
	{
		Enemy->DoRangedAttack();
	}

	if (!bInstantHit || !TargetActor)
	{
		return EBTNodeResult::Succeeded;
	}

	const float Dist = FVector::Dist(SelfPawn->GetActorLocation(), TargetActor->GetActorLocation());
	if (Dist > MaxRange)
	{
		return EBTNodeResult::Succeeded;
	}

	if (ACharacterBase* TargetCharacter = Cast<ACharacterBase>(TargetActor))
	{
		TargetCharacter->ReceiveHit(Damage);
	}

	return EBTNodeResult::Succeeded;
}

