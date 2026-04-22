#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class UInputCacheSystem;

/**
 * 玩家动画实例 (PlayerAnimInstance)
 * 职责: 作为动画图表（AnimGraph）的数据容器，驱动最终的骨骼表现。
 * 说明: 
 * 1. 采用“推(Push)”模式，暴露 BlueprintReadWrite 变量。
 * 2. 这些变量将由 C++ 的具体状态（如 PlayerWalkState）在 Update 时主动赋值。
 * 3. 放弃了在 NativeUpdateAnimation 中主动“拉(Pull)”角色物理状态的做法，实现逻辑状态与动画的强同步。
 */
UCLASS()
class ACTGAME_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 允许状态机直接推送这些变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|State")
	bool IsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|State")
	bool IsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|State")
	bool IsIdleAFK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|State")
	bool IsTurnBack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|State")
	float TurnBackTimer;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
