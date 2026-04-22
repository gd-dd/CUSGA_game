#include "ANS_InputCache.h"
#include "Player/Input/InputCacheSystem.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Character/ACTPlayerController.h"
#include "Player/StateMachine/PlayerStateMachine.h"

void UANS_InputCache::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		{
			if (AACTPlayerController* PC = Cast<AACTPlayerController>(Player->GetController()))
			{
				if (UInputCacheSystem* CacheSystem = PC->InputCacheSystem)
				{
					if (GEngine) GEngine->AddOnScreenDebugMessage(3, 0.2f, FColor::Cyan, TEXT("[Window] AnimCache: OPEN"));
					CacheSystem->SetShouldCache(true);
				}
			}
		}
	}
}

void UANS_InputCache::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_InputCache::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		{
			if (AACTPlayerController* PC = Cast<AACTPlayerController>(Player->GetController()))
			{
				if (UInputCacheSystem* CacheSystem = PC->InputCacheSystem)
				{
					if (GEngine) GEngine->AddOnScreenDebugMessage(3, 0.2f, FColor::Red, TEXT("[Window] AnimCache: CLOSED & Cleared"));
					CacheSystem->SetShouldCache(false);
					CacheSystem->ClearCache();
				}
			}
		}
	}
}
