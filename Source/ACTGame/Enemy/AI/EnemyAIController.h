#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBehaviorTree;

/**
 * 敌人 AIController：负责
 * - 感知玩家（视觉）
 * - 将目标/最后感知位置写入 Blackboard（供行为树决策）
 * - 监听敌人眩晕状态，在眩晕时暂停行为树逻辑，恢复后延迟继续
 */
UCLASS()
class ACTGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	/** 绑定眩晕回调，并在有 DefaultBehaviorTree 时启动行为树 */
	virtual void OnPossess(APawn* InPawn) override;
	/** 解绑回调与定时器清理 */
	virtual void OnUnPossess() override;

	/** 默认行为树资产（通常在蓝图 AIController 上指定） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
	TObjectPtr<UBehaviorTree> DefaultBehaviorTree = nullptr;

	/** Blackboard Key：当前目标（Actor/Object） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
	FName TargetActorKeyName = TEXT("TargetActor");

	/** Blackboard Key：最后一次感知到目标的位置（Vector） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
	FName LastKnownLocationKeyName = TEXT("LastKnownLocation");

	/** 解除眩晕后，恢复行为树逻辑的延迟时间（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|AI")
	float PostStunResumeDelay = 1.5f;

protected:
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig = nullptr;

	/** 感知更新回调：发现/丢失玩家时写入/清空黑板 key */
	UFUNCTION()
	void HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	/** 眩晕变化回调：暂停/恢复行为树逻辑 */
	UFUNCTION()
	void HandleStunChanged(bool bNewStunned);

	/** 解除眩晕延迟结束后，恢复行为树 */
	void ResumeLogicAfterStun();

	FTimerHandle ResumeLogicTimerHandle;
	FDelegateHandle StunDelegateHandle;

	TWeakObjectPtr<class AEnemyBaseCharacter> CachedEnemy;
};

