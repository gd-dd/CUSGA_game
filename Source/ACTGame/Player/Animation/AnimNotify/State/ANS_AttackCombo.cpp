#include "ANS_AttackCombo.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/StateMachine/PlayerStateMachine.h"

void UANS_AttackCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		{
			if (UPlayerStateMachine* StateMachine = Player->GetStateMachine())
			{
				if (GEngine) GEngine->AddOnScreenDebugMessage(3, 0.2f, FColor::Cyan, TEXT("[Window] AttackCombo: OPEN"));
				// 窗口开始时，尝试提取一次连击缓存
				StateMachine->ComboUpdate();
			}
		}
	}
}

void UANS_AttackCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		{
			if (UPlayerStateMachine* StateMachine = Player->GetStateMachine())
			{
				// 在窗口期内持续检查，如果玩家在此期间新按下了攻击键，立刻触发连击
				StateMachine->ComboUpdate();
			}
		}
	}
}
