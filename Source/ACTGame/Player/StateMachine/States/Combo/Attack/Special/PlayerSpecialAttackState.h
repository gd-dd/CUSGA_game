#pragma once

#include "CoreMinimal.h"
#include "Player/StateMachine/States/Combo/Attack/PlayerAttackStateBase.h"
#include "PlayerSpecialAttackState.generated.h"

/**
 * 
 */
UCLASS()
class ACTGAME_API UPlayerSpecialAttackState : public UPlayerAttackStateBase
{
	GENERATED_BODY()

public:
	virtual void EnterState() override;
	virtual void UpdateState(float DeltaTime) override;
	virtual void ExitState() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Attack")
	UAnimMontage* AttackMontage;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
