#pragma once

#include "CoreMinimal.h"
#include "Player/StateMachine/IState.h"
#include "PlayerStateBase.generated.h"

class APlayerCharacter;
class UPlayerStateMachine;
class UInputCacheSystem;
struct FInputActionValue;
class UPlayerAnimInstance;

/**
 * 玩家具体状态基类 (PlayerStateBase)
 * 职责: 为所有具体的玩家状态（如 Idle, Walk, Attack）提供公共的方法和属性。
 * 说明: 
 * 1. 继承自 UObject 以保持轻量，同时实现了 IState 接口。
 * 2. 持有 Character、StateMachine 和 InputCacheSystem 的指针，方便子类调用。
 * 3. 提供了 GetAnimInstance() 便于子类向动画蓝图推送（Push）变量。
 */
UCLASS(Blueprintable, BlueprintType)
class ACTGAME_API UPlayerStateBase : public UObject, public IState
{
	GENERATED_BODY()

public:
	virtual void Init(APlayerCharacter* InCharacter, UPlayerStateMachine* InStateMachine, UInputCacheSystem* InInputCacheSystem);

	// 获取当前状态绑定的动作数据（向绑定的 Character 查询）
	class UActionData* GetActionData() const;

	//~ Begin IState Interface
	virtual void EnterState() override;
	virtual void UpdateState(float DeltaTime) override;
	virtual void ExitState() override;
	//~ End IState Interface

protected:
	// 获取玩家动画实例的快捷方法
	UPlayerAnimInstance* GetAnimInstance() const;

	// 获取输入缓存系统的快捷方法
	UInputCacheSystem* GetInputCacheSystem() const;

	// 检查特定输入动作当前是否被触发（按下）的快捷方法
	bool IsInputActionTriggered(const class UInputAction* Action) const;

	// 获取特定输入动作当前值（Value）的快捷方法，适用于 Axis1D/2D 等
	struct FInputActionValue GetInputActionValue(const class UInputAction* Action) const;

	UPROPERTY()
	APlayerCharacter* Character;

	UPROPERTY()
	UPlayerStateMachine* StateMachine;

	UPROPERTY()
	UInputCacheSystem* InputCacheSystem;
};
