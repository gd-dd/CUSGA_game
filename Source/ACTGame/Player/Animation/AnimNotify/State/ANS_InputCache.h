#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Player/Input/InputCacheSystem.h"
#include "ANS_InputCache.generated.h"

class UInputCacheSystem;
class UPlayerStateBase;

/**
 * 动画通知状态：输入缓存窗口期 (Anim Notify State: Input Cache)
 * 职责: 控制动作游戏中“可预输入”的时间窗口。
 * 说明:
 * 1. 挂载在攻击等动作的 AnimMontage 上。
 * 2. NotifyBegin: 获取角色的 InputCacheSystem 并允许缓存 (bShouldCache = true)。
 * 3. NotifyEnd: 关闭缓存 (bShouldCache = false)。
 * 这样可以精确控制只有在动画播放到特定帧时，玩家按下的下一个攻击键才会被记录。
 */
UCLASS()
class ACTGAME_API UANS_InputCache : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
