#include "Enemy/EnemyBaseCharacter.h"

AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBaseCharacter::SetStunned(bool bNewStunned)
{
	if (bIsStunned == bNewStunned)
	{
		return;
	}

	bIsStunned = bNewStunned;
	OnStunChanged.Broadcast(bIsStunned);
}

void AEnemyBaseCharacter::DoMeleeAttack_Implementation()
{
}

void AEnemyBaseCharacter::DoRangedAttack_Implementation()
{
}

void AEnemyBaseCharacter::DoDash_Implementation()
{
}

