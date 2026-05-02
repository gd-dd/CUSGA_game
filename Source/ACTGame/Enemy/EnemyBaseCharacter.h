#pragma once

#include "CoreMinimal.h"
#include "Player/Base/CharacterBase.h"
#include "EnemyBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyStunChanged, bool, bIsStunned);

UCLASS(Abstract)
class ACTGAME_API AEnemyBaseCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyBaseCharacter();

	UFUNCTION(BlueprintCallable, Category = "Enemy|State")
	void SetStunned(bool bNewStunned);

	UFUNCTION(BlueprintPure, Category = "Enemy|State")
	bool IsStunned() const { return bIsStunned; }

	UPROPERTY(BlueprintAssignable, Category = "Enemy|State")
	FOnEnemyStunChanged OnStunChanged;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|Combat")
	void DoMeleeAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|Combat")
	void DoRangedAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|Combat")
	void DoDash();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|State")
	bool bIsStunned = false;
};

