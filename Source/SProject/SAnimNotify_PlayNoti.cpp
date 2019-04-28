// Fill out your copyright notice in the Description page of Project Settings.

#include "SAnimNotify_PlayNoti.h"
#include "SCharacterBase.h"

void USAnimNotify_PlayNoti::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		ASCharacterBase* SCharacter = Cast<ASCharacterBase>(MeshComp->GetOwner());
		if (SCharacter)
		{
			SCharacter->NotifyAnimationPlayed(NotifyName);
		}
	}
}
