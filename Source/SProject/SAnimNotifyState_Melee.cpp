// Fill out your copyright notice in the Description page of Project Settings.

#include "SAnimNotifyState_Melee.h"
#include "SCharacterBase.h"

void USAnimNotifyState_Melee::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp)
	{
		ASCharacterBase* SCharacter = Cast<ASCharacterBase>(MeshComp->GetOwner());
		if (SCharacter)
		{
			SCharacter->SetWeaponCollision(WeaponCollType, true);
		}
	}

}

void USAnimNotifyState_Melee::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp)
	{
		ASCharacterBase* SCharacter = Cast<ASCharacterBase>(MeshComp->GetOwner());
		if (SCharacter)
		{
			SCharacter->SetWeaponCollision(WeaponCollType, false);
		}
	}
}
