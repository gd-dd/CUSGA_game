#pragma once

#include "CoreMinimal.h"
#include "Player/StateMachine/States/Combo/Attack/PlayerAttackStateBase.h"
#include "PlayerEvadeState.generated.h"

class UAnimMontage;

/**
 * 玩家闪避状态
 */
UCLASS(Blueprintable)
class ACTGAME_API UPlayerEvadeState : public UPlayerAttackStateBase
{
	GENERATED_BODY()

public:
	UPlayerEvadeState();

	virtual void EnterState() override;
	virtual void UpdateState(float DeltaTime) override;
	virtual void ExitState() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animation")
	UAnimMontage* AM_Evade_Front;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animation")
	UAnimMontage* AM_Evade_Back;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Movement")
	float CharacterRotateSpeed = 10.0f;

private:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 是否需要跟随摇杆输入平滑转向
	bool bShouldRotate;
};
