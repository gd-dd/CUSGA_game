#include "Enemy/AI/BT/BTTask_DashLastKnownDirection.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

namespace
{
	struct FDashTaskMemory
	{
		float EndTime = 0.0f;
		bool bStarted = false;
	};
}

UBTTask_DashLastKnownDirection::UBTTask_DashLastKnownDirection()
{
	NodeName = TEXT("Dash Last Known Direction");
	bNotifyTick = true;
}

uint16 UBTTask_DashLastKnownDirection::GetInstanceMemorySize() const
{
	return sizeof(FDashTaskMemory);
}

EBTNodeResult::Type UBTTask_DashLastKnownDirection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FDashTaskMemory* Mem = (FDashTaskMemory*)NodeMemory;
	Mem->bStarted = false;

	AAIController* AICon = OwnerComp.GetAIOwner();
	ACharacter* SelfChar = AICon ? Cast<ACharacter>(AICon->GetPawn()) : nullptr;
	if (!SelfChar)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}

	const FVector LastKnownLocation = BB->GetValueAsVector(LastKnownLocationKey.SelectedKeyName);
	FVector Dir = (LastKnownLocation - SelfChar->GetActorLocation()).GetSafeNormal2D();
	if (Dir.IsNearlyZero())
	{
		Dir = SelfChar->GetActorForwardVector().GetSafeNormal2D();
	}

	SelfChar->SetActorRotation(Dir.Rotation());

	if (AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(SelfChar))
	{
		Enemy->DoDash();
	}

	SelfChar->LaunchCharacter(Dir * DashSpeed, true, true);

	const float Now = SelfChar->GetWorld() ? SelfChar->GetWorld()->GetTimeSeconds() : 0.0f;
	Mem->EndTime = Now + DashDuration;
	Mem->bStarted = true;
	return EBTNodeResult::InProgress;
}

void UBTTask_DashLastKnownDirection::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FDashTaskMemory* Mem = (FDashTaskMemory*)NodeMemory;
	AAIController* AICon = OwnerComp.GetAIOwner();
	ACharacter* SelfChar = AICon ? Cast<ACharacter>(AICon->GetPawn()) : nullptr;
	if (!SelfChar || !Mem->bStarted)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const float Now = SelfChar->GetWorld() ? SelfChar->GetWorld()->GetTimeSeconds() : 0.0f;
	if (Now < Mem->EndTime)
	{
		return;
	}

	if (UCharacterMovementComponent* MoveComp = SelfChar->GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

