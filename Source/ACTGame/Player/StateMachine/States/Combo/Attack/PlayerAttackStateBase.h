#pragma once

#include "CoreMinimal.h"
#include "Player/StateMachine/States/PlayerStateBase.h"
#include "PlayerAttackStateBase.generated.h"

UCLASS()
class ACTGAME_API UPlayerAttackStateBase : public UPlayerStateBase
{
	GENERATED_BODY()

public:
	UPlayerAttackStateBase();

	virtual void Enter() override;
	virtual void Exit() override;

	void SetCanMontageExit(bool bInCanMontageExit);

	bool GetCanMontageExit() const;

protected:
	UFUNCTION()
	void HandleHitTarget(const FHitResult& HitObject);

protected:
	bool bCanMontageExit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Damage")
	float BaseDamage = 10.0f;
};

