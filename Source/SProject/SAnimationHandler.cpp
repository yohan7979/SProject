// Fill out your copyright notice in the Description page of Project Settings.

#include "SAnimationHandler.h"
#include "Animation/AnimMontage.h"

USAnimationHandler::USAnimationHandler()
{
	AnimMontageList.Add(EAnimMontageType::EAMT_NormalAttack_A, nullptr);
	AnimMontageList.Add(EAnimMontageType::EAMT_NormalAttack_B, nullptr);
	AnimMontageList.Add(EAnimMontageType::EAMT_NormalAttack_C, nullptr);
	AnimMontageList.Add(EAnimMontageType::EAMT_NormalAttack_D, nullptr);
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

	if (AnimMontageList[eAnimType] != nullptr)
	{
		CachedCharacter->PlayAnimMontage(AnimMontageList[eAnimType]);
	}
}

