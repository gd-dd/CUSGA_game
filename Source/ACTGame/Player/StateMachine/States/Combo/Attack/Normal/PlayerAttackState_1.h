#pragma once

#include "CoreMinimal.h"
#include "Player/StateMachine/States/Combo/Attack/PlayerAttackStateBase.h"
#include "PlayerAttackState_1.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class ACTGAME_API UPlayerAttackState_1 : public UPlayerAttackStateBase
{
	GENERATED_BODY()

public:
	UPlayerAttackState_1();
	
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animation")
	UAnimMontage* AttackMontage;

private:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};

