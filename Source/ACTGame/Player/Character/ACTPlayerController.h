﻿﻿﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACTPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class APlayerCharacter;

/**
 * 基础玩家控制器类 (PlayerController)
 * 职责: 负责接收玩家的物理外设输入（键盘、鼠标、手柄），并将指令下发给当前控制的角色。
 * 说明: 在此统一配置和绑定 Enhanced Input 系统的输入映射上下文 (IMC) 和具体的 InputAction。
 */
UCLASS()
class ACTGAME_API AACTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AACTPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	// 获取当前控制的角色
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	APlayerCharacter* CurrentPlayerCharacter;
	
	// 基础的输入映射上下文
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* IMC_ACT;
	
	// 移动
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_ACT_Move;

	// 镜头
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_ACT_Look;
	
	// 攻击输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_NormalAttack;
	
	// 特殊攻击输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_SpecialAttack;

	// 闪避输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Evade;

public:
	// 输入缓存系统组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInputCacheSystem* InputCacheSystem;

	// 获取增强输入动作是否处于激活/触发状态（适用于判断按键是否按住）
	bool IsInputActionTriggered(const UInputAction* Action) const;
	
	// 获取增强输入动作的具体值（适用于获取摇杆/鼠标移动的 Axis 值）
	FInputActionValue GetInputActionValue(const UInputAction* Action) const;
	
	// 供外部获取具体的 InputAction 指针，以便查询
	UInputAction* GetAttackAction() const { return IA_NormalAttack; }
	UInputAction* GetMoveAction() const { return IA_ACT_Move; }
	
private:

	// 声明移动和视角移动的函数
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// 攻击函数，处理输入缓存
	void NormalAttack();
	void SpecialAttack();

	// 闪避函数
	void Evade();

	// 闪避冷却计时器
	float EvadeCooldownTimer;
	
	// 闪避冷却时间（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Evade", meta = (AllowPrivateAccess = "true"))
	float EvadeCooldownDuration = 1.0f;
};

