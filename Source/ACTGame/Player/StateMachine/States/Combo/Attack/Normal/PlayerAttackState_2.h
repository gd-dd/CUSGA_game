#pragma once

#include "CoreMinimal.h"
#include "Player/StateMachine/States/Combo/Attack/PlayerAttackStateBase.h"
#include "PlayerAttackState_2.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class ACTGAME_API UPlayerAttackState_2 : public UPlayerAttackStateBase
{
	GENERATED_BODY()

public:
	UPlayerAttackState_2();
	
	virtual void EnterState() override;
	virtual void UpdateState(float DeltaTime) override;
	virtual void ExitState() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animation")
	UAnimMontage* AttackMontage;

private:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
