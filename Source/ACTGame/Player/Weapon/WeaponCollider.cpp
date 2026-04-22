#include "WeaponCollider.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetSystemLibrary.h"

UWeaponCollider::UWeaponCollider()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponCollider::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!isCollider || !WeaponComponent || !GetWorld())
	{
		return;
	}

	const FVector Start = WeaponComponent->GetSocketLocation(StartSocket);
	const FVector End = WeaponComponent->GetSocketLocation(StopSocket);

	TArray<AActor*> ActorsToIgnore;
	if (AActor* Owner = GetOwner())
	{
		ActorsToIgnore.Add(Owner);
	}

	TArray<FHitResult> OutHits;
	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		Start,
		End,
		TraceRadius,
		TraceObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		OutHits,
		true
	);

	if (!bHit)
	{
		return;
	}

	for (const FHitResult& OutHit : OutHits)
	{
		const FHitResult HitObject = OutHit;
		AActor* HitActor = HitObject.GetActor();
		if (!HitActor)
		{
			continue;
		}

		if (ColliderObjects.Contains(HitActor))
		{
			continue;
		}

		ColliderObjects.Add(HitActor);
		HitTarget.Broadcast(HitObject);
	}
}

void UWeaponCollider::SetWeaponComponent(UStaticMeshComponent* Weapon)
{
	WeaponComponent = Weapon;
}

void UWeaponCollider::EnableCollider()
{
	ClearCollider();
	isCollider = true;
}

void UWeaponCollider::DisableCollider()
{
	if (isCollider)
	{
		isCollider = false;
	}
}

void UWeaponCollider::ClearCollider()
{
	ColliderObjects.Empty();
}
