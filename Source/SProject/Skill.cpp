// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill.h"
#include "SAttributeComponent.h"
#include "SCharacterBase.h"

// Sets default values for this component's properties
USkill::USkill()
{

}

bool USkill::InCooldown(ASCharacterBase* Owner)
{
	if (Owner)
	{
		return Owner->GetWorldTimerManager().IsTimerActive(TimerHandle_Cooldown);
	}
	return false;
}

void USkill::SetCooldown(ASCharacterBase* Owner)
{
	if (Owner)
	{
		Owner->GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, Cooldown, false);
	}
}
