#include "PlayerAttackStateBase.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Weapon/WeaponCollider.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Base/GamePlay/ACTPlayerState.h"
#include "Tools/Log/PlayerDebug.h"

UPlayerAttackStateBase::UPlayerAttackStateBase()
{
	bCanMontageExit = false;
}

void UPlayerAttackStateBase::Enter()
{
	Super::Enter();

	if (!Character)
	{
		return;
	}

	if (UWeaponCollider* WeaponCollider = Character->FindComponentByClass<UWeaponCollider>())
	{
		WeaponCollider->HitTarget.RemoveDynamic(this, &UPlayerAttackStateBase::HandleHitTarget);
		WeaponCollider->HitTarget.AddDynamic(this, &UPlayerAttackStateBase::HandleHitTarget);
	}
}

void UPlayerAttackStateBase::Exit()
{
	if (Character)
	{
		if (UWeaponCollider* WeaponCollider = Character->FindComponentByClass<UWeaponCollider>())
		{
			WeaponCollider->HitTarget.RemoveDynamic(this, &UPlayerAttackStateBase::HandleHitTarget);
		}
	}

	Super::Exit();
}

void UPlayerAttackStateBase::SetCanMontageExit(bool bInCanMontageExit)
{
	bCanMontageExit = bInCanMontageExit;
}

bool UPlayerAttackStateBase::GetCanMontageExit() const
{
	return bCanMontageExit;
}

void UPlayerAttackStateBase::HandleHitTarget(const FHitResult& HitObject)
{
	if (!Character)
	{
		return;
	}

	AActor* DamagedActor = HitObject.GetActor();
	if (!DamagedActor || DamagedActor == Character)
	{
		return;
	}

	FVector HitFromDirection = Character->GetActorForwardVector();
	if (UWeaponCollider* WeaponCollider = Character->FindComponentByClass<UWeaponCollider>())
	{
		if (UPrimitiveComponent* WeaponComp = WeaponCollider->GetWeaponComponent())
		{
			HitFromDirection = WeaponComp->GetForwardVector();
		}
	}

	UGameplayStatics::ApplyPointDamage(
		DamagedActor,
		BaseDamage,
		HitFromDirection,
		HitObject,
		Character->GetController(),
		Character,
		nullptr
	);

	// 记录玩家战斗数据
	if (AACTPlayerState* PS = Character->GetPlayerState<AACTPlayerState>())
	{
		PS->RecordHit(BaseDamage);
	}

	DEBUG_PRINT_LOG(-1, FColor::Yellow, TEXT("[AttackHit] Target=%s Damage=%.1f Direction=%s"),
		*DamagedActor->GetName(),
		BaseDamage,
		*HitFromDirection.ToString()
	);
}


