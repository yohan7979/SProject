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

void USAnimationHandler::PlayAnimationMontage(EAnimMontageType eAnimType)
{
	if (CachedCharacter == nullptr)
		return;

	UAnimMontage* DesireToPlay = *AnimMontageList.Find(eAnimType);
	if (DesireToPlay != nullptr)
	{
		CachedCharacter->PlayAnimMontage(DesireToPlay);
	}
}

