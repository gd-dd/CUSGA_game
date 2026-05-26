#include "Enemy/EnemyBaseCharacter.h"

AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Health = MaxHealth;
}

void AEnemyBaseCharacter::ReceiveHit(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health <= 0.0f)
	{
		Health = 0.0f;
		// 此处可以添加敌人死亡逻辑
	}
}

void AEnemyBaseCharacter::SetStunned(bool bNewStunned)
{
	// 眩晕状态未变化则不重复广播
	if (bIsStunned == bNewStunned)
	{
		return;
	}

	bIsStunned = bNewStunned;
	// 通知监听者（例如 AIController）进行行为树暂停/恢复等处理
	OnStunChanged.Broadcast(bIsStunned);
}

void AEnemyBaseCharacter::DoMeleeAttack_Implementation()
{
	// 默认不做任何事：建议在蓝图中覆写播放近战攻击动画/特效
}

void AEnemyBaseCharacter::DoRangedAttack_Implementation()
{
	// 默认不做任何事：建议在蓝图中覆写生成投射物/播放远程攻击动画
}

void AEnemyBaseCharacter::DoDash_Implementation()
{
	// 默认不做任何事：建议在蓝图中覆写冲刺动画/特效/碰撞逻辑
}

