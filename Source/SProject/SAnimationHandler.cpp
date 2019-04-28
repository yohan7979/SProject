// Fill out your copyright notice in the Description page of Project Settings.

#include "SAnimationHandler.h"
#include "Animation/AnimMontage.h"
#include "SCharacterBase.h"

USAnimationHandler::USAnimationHandler()
{
}


// Called when the game starts
void USAnimationHandler::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacter = Cast<ASCharacterBase>(GetOwner());
}

void USAnimationHandler::PlayAnimationMontage(EAnimMontageType eAnimType, FName SectionName, bool bReverse)
{
	if (CachedCharacter == nullptr)
		return;

	UAnimMontage** DesireToPlay = AnimMontageList.Find(eAnimType);
	if (DesireToPlay != nullptr)
	{
		CachedCharacter->PlayAnimMontage(*DesireToPlay, bReverse ? -1.f : 1.f, SectionName);
	}
	else
	{
		UAnimMontage* CurrentMontage = CachedCharacter->GetCurrentMontage();
		if (CurrentMontage != nullptr)
		{
			CachedCharacter->StopAnimMontage(CurrentMontage);
		}
	}
}

void USAnimationHandler::StopAnimationMontage(EAnimMontageType eAnimType)
{
	if (CachedCharacter == nullptr)
		return;

	UAnimMontage** DesireToPlay = AnimMontageList.Find(eAnimType);
	if (DesireToPlay != nullptr)
	{
		CachedCharacter->StopAnimMontage(*DesireToPlay);
	}
}

bool USAnimationHandler::IsMontangePlaying(EAnimMontageType eAnimType)
{
	if (CachedCharacter == nullptr)
		return false;

	UAnimMontage* CurrentMontage = CachedCharacter->GetCurrentMontage();
	UAnimMontage** pTargetMontage = AnimMontageList.Find(eAnimType);
	if (CurrentMontage && pTargetMontage)
	{
		if (CurrentMontage == *pTargetMontage)
		{
			return true;
		}
	}

	return false;
}
