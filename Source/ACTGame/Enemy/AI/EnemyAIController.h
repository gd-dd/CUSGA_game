#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBehaviorTree;

UCLASS()
class ACTGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
	TObjectPtr<UBehaviorTree> DefaultBehaviorTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
	FName TargetActorKeyName = TEXT("TargetActor");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
	FName LastKnownLocationKeyName = TEXT("LastKnownLocation");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
	float PostStunResumeDelay = 1.5f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|AI")
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig = nullptr;

	UFUNCTION()
	void HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void HandleStunChanged(bool bNewStunned);

	void ResumeLogicAfterStun();

	FTimerHandle ResumeLogicTimerHandle;
	FDelegateHandle StunDelegateHandle;

	TWeakObjectPtr<class AEnemyBaseCharacter> CachedEnemy;
};

