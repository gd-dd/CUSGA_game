#include "ACTPlayerState.h"
#include "Engine/World.h"
#include "Tools/Log/PlayerDebug.h"

AACTPlayerState::AACTPlayerState()
{
	// 默认不需要每帧 Tick
	PrimaryActorTick.bCanEverTick = false;
	
	// 初始化数据
	CurrentCombo = 0;
	TotalDamage = 0.0f;
	CombatStartTime = 0.0f;
	bIsInCombat = false;
}

void AACTPlayerState::BeginPlay()
{
	Super::BeginPlay();
	ResetCombatStats();
}

void AACTPlayerState::RecordHit(float DamageAmount)
{
	// 如果是战斗中的第一击，记录战斗开始时间
	if (!bIsInCombat)
	{
		bIsInCombat = true;
		if (GetWorld())
		{
			CombatStartTime = GetWorld()->GetTimeSeconds();
		}
	}

	CurrentCombo++;
	TotalDamage += DamageAmount;
	
	// 可选的调试输出
	DEBUG_PRINT_LOG(-1, FColor::Yellow, TEXT("Hit! Combo: %d | DMG: %.1f | Total DMG: %.1f | DPS: %.1f"), 
		CurrentCombo, DamageAmount, TotalDamage, GetDPS());
}

void AACTPlayerState::ResetCombo()
{
	CurrentCombo = 0;
	
	// 这里可以视游戏设计决定：如果断连，是否也停止战斗计秒，重新计算下一段DPS。
	// 目前仅重置连击数，保留总伤害和整场DPS。
}

void AACTPlayerState::ResetCombatStats()
{
	CurrentCombo = 0;
	TotalDamage = 0.0f;
	CombatStartTime = 0.0f;
	bIsInCombat = false;
}

float AACTPlayerState::GetDPS() const
{
	if (!bIsInCombat || !GetWorld()) return 0.0f;

	float CurrentTime = GetWorld()->GetTimeSeconds();
	float CombatDuration = CurrentTime - CombatStartTime;
	
	// 防止极短时间内的除零/数值爆炸
	if (CombatDuration < 0.1f)
	{
		return TotalDamage; // 瞬间爆发按总伤害算
	}

	return TotalDamage / CombatDuration;
}

