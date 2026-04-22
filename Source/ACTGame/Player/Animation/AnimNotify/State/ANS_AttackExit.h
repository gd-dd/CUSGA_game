#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AttackExit.generated.h"

/**
 * 动画通知状态：攻击退出/打断窗口期 (Anim Notify State: Attack Exit)
 * 职责: 控制动作游戏中攻击“后摇”可被打断的时间窗口。
 * 说明:
 * 1. 挂载在攻击 AnimMontage 的末尾部分。
 * 2. 当处于这个通知区间内时，意味着“伤害已经打出去了”，此时允许玩家用移动、闪避等动作强制取消当前的后摇动画。
 */
UCLASS()
class ACTGAME_API UANS_AttackExit : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
