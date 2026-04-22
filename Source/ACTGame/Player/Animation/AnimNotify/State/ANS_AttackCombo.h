#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AttackCombo.generated.h"

/**
 * 动画通知状态：连击判定窗口 (Anim Notify State: Attack Combo)
 * 职责: 替代原蓝图的 ANS_AttackReinvoke，在一个时间窗口内持续检测是否有攻击缓存，
 * 如果有，则立刻触发下一段攻击连击。
 */
UCLASS()
class ACTGAME_API UANS_AttackCombo : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
};
