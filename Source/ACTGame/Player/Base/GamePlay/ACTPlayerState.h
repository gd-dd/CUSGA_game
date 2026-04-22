#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ACTPlayerState.generated.h"

/**
 * 玩家状态类 (框架级)
 * 伴随玩家的连接存在，即使控制的角色(Pawn)死亡或切换，该状态依然保留。
 * 适用于记录单局游戏中的玩家个人数据，如连击数、得分、评价等级等。
 */
UCLASS()
class ACTGAME_API AACTPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AACTPlayerState();

protected:
	virtual void BeginPlay() override;

public:
	// --- 核心战斗数据 API ---

	// 记录一次新的命中，增加连击数和总伤害
	UFUNCTION(BlueprintCallable, Category = "Combat Stats")
	void RecordHit(float DamageAmount);

	// 重置连击数（例如：一段时间未攻击，或者受到攻击时）
	UFUNCTION(BlueprintCallable, Category = "Combat Stats")
	void ResetCombo();

	// 重置所有战斗数据（重新开始战斗时）
	UFUNCTION(BlueprintCallable, Category = "Combat Stats")
	void ResetCombatStats();

	// --- 数据获取 API ---

	UFUNCTION(BlueprintPure, Category = "Combat Stats")
	int32 GetComboCount() const { return CurrentCombo; }

	UFUNCTION(BlueprintPure, Category = "Combat Stats")
	float GetTotalDamage() const { return TotalDamage; }

	// 计算当前的 DPS (TotalDamage / CombatDuration)
	UFUNCTION(BlueprintPure, Category = "Combat Stats")
	float GetDPS() const;

protected:
	// --- 战斗数据 ---

	// 当前连击数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat Stats")
	int32 CurrentCombo;

	// 造成的总伤害
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat Stats")
	float TotalDamage;

	// 战斗开始的时间戳（用于计算 DPS）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat Stats")
	float CombatStartTime;

	// 是否处于战斗状态
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat Stats")
	bool bIsInCombat;
};
