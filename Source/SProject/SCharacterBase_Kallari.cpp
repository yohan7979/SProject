// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacterBase_Kallari.h"
#include "SAnimationHandler.h"

ASCharacterBase_Kallari::ASCharacterBase_Kallari()
{
	LeftWeaponCollComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftWeaponCollComp"));
	LeftWeaponCollComp->SetupAttachment(GetMesh(), "WeaponAttachPointL");

	RightWeaponCollComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightWeaponCollComp"));
	RightWeaponCollComp->SetupAttachment(GetMesh(), "WeaponAttachPointR");
}

void ASCharacterBase_Kallari::NormalAttack()
{
	if (AnimationHandler != nullptr)
	{
		AnimationHandler->PlayAnimationMontage(EAMT_Melee);
	}
}
