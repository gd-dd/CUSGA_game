#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponCollider.generated.h"

class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitTargetSignature, const FHitResult&, HitObject);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTGAME_API UWeaponCollider : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponCollider();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetWeaponComponent(UStaticMeshComponent* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Combat|WeaponCollider")
	void EnableCollider();

	UFUNCTION(BlueprintCallable, Category = "Combat|WeaponCollider")
	void DisableCollider();

	UFUNCTION(BlueprintCallable, Category = "Combat|WeaponCollider")
	void ClearCollider();

	UStaticMeshComponent* GetWeaponComponent() const { return WeaponComponent; }

public:
	UPROPERTY(BlueprintAssignable, Category = "Combat|WeaponCollider")
	FOnHitTargetSignature HitTarget;

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|WeaponCollider")
	bool isCollider = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|WeaponCollider")
	TArray<TObjectPtr<AActor>> ColliderObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|WeaponCollider")
	FName StartSocket = TEXT("StartSocket");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|WeaponCollider")
	FName StopSocket = TEXT("StopSocket");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|WeaponCollider")
	float TraceRadius = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|WeaponCollider")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
};
