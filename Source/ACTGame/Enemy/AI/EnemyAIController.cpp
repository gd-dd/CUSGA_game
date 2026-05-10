#include "Enemy/AI/EnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"

AEnemyAIController::AEnemyAIController()
{
	// 初始化感知组件（AI Perception）
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*PerceptionComponent);

	// 配置视觉感知：半径/丢失半径/视野角/记忆时长
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 2500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
	SightConfig->SetMaxAge(2.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	// 感知到目标变化时回调，用于写入黑板 TargetActor/LastKnownLocation
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::HandlePerceptionUpdated);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 缓存敌人 Pawn，并监听眩晕状态：眩晕时暂停行为树逻辑
	CachedEnemy = Cast<AEnemyBaseCharacter>(InPawn);
	if (CachedEnemy.IsValid())
	{
		CachedEnemy->OnStunChanged.AddDynamic(this, &AEnemyAIController::HandleStunChanged);
	}

	// 启动默认行为树（通常由蓝图 AIController 指定）
	if (DefaultBehaviorTree)
	{
		RunBehaviorTree(DefaultBehaviorTree);
	}
}

void AEnemyAIController::OnUnPossess()
{
	// 清理恢复逻辑的定时器，避免解除占用后仍触发回调
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ResumeLogicTimerHandle);
	}

	// 解绑眩晕回调
	if (CachedEnemy.IsValid())
	{
		CachedEnemy->OnStunChanged.RemoveDynamic(this, &AEnemyAIController::HandleStunChanged);
		CachedEnemy.Reset();
	}

	Super::OnUnPossess();
}

void AEnemyAIController::HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	// 当前实现只关心 0 号玩家
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
	{
		return;
	}

	// 行为树侧依赖黑板数据做决策
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB)
	{
		return;
	}

	for (AActor* Actor : UpdatedActors)
	{
		// 只处理玩家目标
		if (Actor != PlayerPawn)
		{
			continue;
		}

		FActorPerceptionBlueprintInfo Info;
		PerceptionComponent->GetActorsPerception(Actor, Info);

		bool bSensed = false;
		FVector LastLocation = Actor->GetActorLocation();
		// 从刺激信息中取出“视觉”这一类刺激
		for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
		{
			if (Stimulus.Type == SightConfig->GetSenseID())
			{
				bSensed = Stimulus.WasSuccessfullySensed();
				LastLocation = Stimulus.StimulusLocation;
				break;
			}
		}

		if (bSensed)
		{
			// 成功看到玩家：设置目标，并更新最后位置
			BB->SetValueAsObject(TargetActorKeyName, Actor);
			BB->SetValueAsVector(LastKnownLocationKeyName, Actor->GetActorLocation());
		}
		else
		{
			// 丢失目标：清空目标，但保留最后一次刺激位置（供追踪/冲刺等使用）
			BB->ClearValue(TargetActorKeyName);
			BB->SetValueAsVector(LastKnownLocationKeyName, LastLocation);
		}
	}
}

void AEnemyAIController::HandleStunChanged(bool bNewStunned)
{
	if (!BrainComponent)
	{
		return;
	}

	// 状态变化时先清掉旧定时器，避免重复恢复
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ResumeLogicTimerHandle);
	}

	if (bNewStunned)
	{
		// 眩晕：停止移动并暂停行为树（不再执行 MoveTo/攻击等）
		StopMovement();
		BrainComponent->PauseLogic(TEXT("Stunned"));
	}
	else
	{
		// 解除眩晕：延迟恢复逻辑（对齐“跪倒结束后再继续”的需求）
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(ResumeLogicTimerHandle, this, &AEnemyAIController::ResumeLogicAfterStun, PostStunResumeDelay, false);
		}
	}
}

void AEnemyAIController::ResumeLogicAfterStun()
{
	if (BrainComponent)
	{
		// 恢复行为树逻辑
		BrainComponent->ResumeLogic(TEXT("StunnedEnded"));
	}
}

