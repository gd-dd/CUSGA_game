#include "ANS_AttackExit.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Character/ACTPlayerController.h"
#include "Player/StateMachine/PlayerStateMachine.h"
#include "Player/StateMachine/States/Combo/Attack/PlayerAttackStateBase.h"
#include "Player/StateMachine/States/Locomotion/PlayerWalkState.h"
#include "Player/StateMachine/States/Locomotion/PlayerIdleState.h"

void UANS_AttackExit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		{
			if (UPlayerStateMachine* StateMachine = Player->GetStateMachine())
			{
				if (UPlayerAttackStateBase* AttackState = Cast<UPlayerAttackStateBase>(StateMachine->GetCurrentState()))
				{
					AttackState->SetCanMontageExit(true);
				}
			}
		}
	}
}

void UANS_AttackExit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		{
			if (AACTPlayerController* PC = Cast<AACTPlayerController>(Player->GetController()))
			{
				// 获取移动输入 Action
				if (UInputAction* MoveAction = PC->GetMoveAction())
				{
					// 如果检测到玩家有移动输入（摇杆推了或者按了方向键）
					if (PC->IsInputActionTriggered(MoveAction))
					{
						if (UPlayerStateMachine* StateMachine = Player->GetStateMachine())
						{
							if (GEngine) GEngine->AddOnScreenDebugMessage(4, 2.0f, FColor::Purple, TEXT("[Event] Attack Interrupted by Movement!"));
							
							// 停止当前正在播放的攻击蒙太奇
							Player->StopAnimMontage();
							
							// 强制状态机切入 Walk 状态，恢复正常移动逻辑
							StateMachine->EnterState(UPlayerWalkState::StaticClass());
						}
					}
				}
			}
		}
	}
}

void UANS_AttackExit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		{
			if (UPlayerStateMachine* StateMachine = Player->GetStateMachine())
			{
				if (UPlayerAttackStateBase* AttackState = Cast<UPlayerAttackStateBase>(StateMachine->GetCurrentState()))
				{
					AttackState->SetCanMontageExit(false);
				}
			}
		}
	}
}
