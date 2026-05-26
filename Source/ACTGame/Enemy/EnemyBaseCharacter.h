#pragma once

#include "CoreMinimal.h"
#include "Player/Base/CharacterBase.h"
#include "EnemyBaseCharacter.generated.h"

/**
 * 敌人眩晕状态变化委托
 * @param bIsStunned true=进入眩晕/跪倒等失能状态；false=恢复
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyStunChanged, bool, bIsStunned);

/**
 * 敌人基类（C++ 侧提供通用状态与 AI/行为树调用入口）
 *
 * 设计要点：
 * - 眩晕状态由 SetStunned 驱动，并通过 OnStunChanged 通知 AIController 暂停/恢复行为树逻辑
 * - DoMeleeAttack/DoRangedAttack/DoDash 为 BlueprintNativeEvent：默认实现为空，建议在蓝图覆写实现动画/特效/生成投射物等
 */
UCLASS(Abstract)
class ACTGAME_API AEnemyBaseCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyBaseCharacter();
	virtual void ReceiveHit(float DamageAmount) override;

	/** 设置眩晕状态（会广播 OnStunChanged） */
	UFUNCTION(BlueprintCallable, Category = "Enemy|State")
	void SetStunned(bool bNewStunned);

	UFUNCTION(BlueprintPure, Category = "Enemy|Health")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category = "Enemy|Health")
	float GetMaxHealth() const { return MaxHealth; }

	/** 是否处于眩晕状态 */
	UFUNCTION(BlueprintPure, Category = "Enemy|State")
	bool IsStunned() const { return bIsStunned; }

	/** 眩晕状态变化事件（AIController 通常会监听并暂停/恢复行为树） */
	UPROPERTY(BlueprintAssignable, Category = "Enemy|State")
	FOnEnemyStunChanged OnStunChanged;

	/** 近战攻击入口：行为树/AI 调用，具体表现通常在蓝图覆写 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|Combat")
	void DoMeleeAttack();

	/** 远程攻击入口：行为树/AI 调用，具体表现通常在蓝图覆写 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|Combat")
	void DoRangedAttack();

	/** 冲刺/冲锋入口：行为树/AI 调用，具体表现通常在蓝图覆写 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|Combat")
	void DoDash();

protected:
	/** 当前生命值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Health")
	float Health = 100.0f;

	/** 最大生命值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Health", meta = (ClampMin = "0.1"))
	float MaxHealth = 100.0f;

	/** 当前眩晕状态（由 SetStunned 修改） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|State")
	bool bIsStunned = false;
};

