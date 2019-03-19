// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill_Toggle.h"
#include "SCharacterBase.h"

bool USkill_Toggle::InCooldown(ASCharacterBase* Owner)
{
	if (Owner)
	{
		if (!bCasting)
		{
			return Owner->GetWorldTimerManager().IsTimerActive(TimerHandle_Cooldown);
		}
		else
		{
			return Owner->GetWorldTimerManager().IsTimerActive(TimerHandle_SecondCooldown);
		}
		
	}
	return false;
}

void USkill_Toggle::SetCooldown(ASCharacterBase* Owner)
{
	if (Owner)
	{
		if (!bCasting)
		{
			Owner->GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, Cooldown, false);
		}
		else
		{
			Owner->GetWorldTimerManager().SetTimer(TimerHandle_SecondCooldown, SecondCooldown, false);
		}
	}
}

void USkill_Toggle::SetActivate(ASCharacterBase* Owner)
{
	if (!bCasting)
	{
		Super::SetActivate(Owner);
		bCasting = true;
	}
	else
	{
		ClearActivate(Owner);
		bCasting = false;
	}
}

void USkill_Toggle::ClearActivate(ASCharacterBase* Owner)
{
	Super::ClearActivate(Owner);

	bCasting = false;
}

float USkill_Toggle::GetCooldown() const
{
	if (!bCasting)
	{
		return Cooldown;
	}
	else
	{
		return SecondCooldown;
	}
}

float USkill_Toggle::GetManaCost() const
{
	if (!bCasting)
	{
		return ManaCost;
	}
	else
	{
		return SecondManaCost;
	}
}

void USkill_Toggle::NotifyFinishToOwner(ASCharacterBase* Owner)
{
	Super::NotifyFinishToOwner(Owner);

	bCasting = false;
}
