// Fill out your copyright notice in the Description page of Project Settings.

#include "SAbilityComponent.h"
#include "Skill.h"
#include "SCharacterBase.h"
#include "SAttributeComponent.h"

USAbilityComponent::USAbilityComponent()
{
	CachedPawn = Cast<ASCharacterBase>(GetOwner());
}

void USAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto it : SkillSlotClasses)
	{
		ESkillType SkillType = it.Key;
		if (it.Value != nullptr)
		{
			USkill* SkillSlot = it.Value->GetDefaultObject<USkill>();
			AddSkillSlot(SkillType, SkillSlot);
			SkillSlot->SetThisSkillType(SkillType);
		}
	}
	
}

void USAbilityComponent::AddSkillSlot(ESkillType SkillType, USkill* NewSkill)
{
	RemoveSkillSlot(SkillType);

	SkillSlots.Add(SkillType, NewSkill);
}

void USAbilityComponent::RemoveSkillSlot(ESkillType SkillType)
{
	USkill** TargetSkill = SkillSlots.Find(SkillType);

	if (TargetSkill)
	{
		SkillSlots.Remove(SkillType);
	}
}

bool USAbilityComponent::CanExecute(ESkillType SkillType)
{
	USkill** TargetSkill = SkillSlots.Find(SkillType);
	if (TargetSkill && CachedPawn)
	{
		if (CheckConditions(*TargetSkill) == true)
		{
			return true;
		}
	}
	return false;
}

void USAbilityComponent::ExecuteSkill(ESkillType SkillType)
{
	USkill** TargetSkill = SkillSlots.Find(SkillType);
	if (TargetSkill && CachedPawn)
	{
		// 발동 조건 성립시, 코스트 계산 수행
		SetSkillCost(*TargetSkill);
	}
}

void USAbilityComponent::CancelSkill(ESkillType SkillType)
{
	USkill** TargetSkill = SkillSlots.Find(SkillType);
	if (TargetSkill && CachedPawn)
	{
		(*TargetSkill)->ClearActivate(CachedPawn);
	}
}

bool USAbilityComponent::CheckConditions(USkill* TargetSkill)
{
	// 마나 코스트 검사
	if (!CheckManaCost(TargetSkill))
		return false;

	// 타 스킬 발동 중인지 검사
	if (!CheckCanActivate(TargetSkill))
		return false;

	// 쿨다운 검사
	if (!CheckCooldown(TargetSkill))
		return false;

	return true;
}

bool USAbilityComponent::CheckCooldown(USkill* TargetSkill)
{
	if (TargetSkill && !TargetSkill->InCooldown(CachedPawn))
	{
		return true;
	}

	return false;
}

bool USAbilityComponent::CheckManaCost(USkill* TargetSkill)
{
	USAttributeComponent* AttributeComp = CachedPawn->GetAttributeComp();
	if (TargetSkill && AttributeComp)
	{
		return AttributeComp->GetCurrentMana() - TargetSkill->GetManaCost() >= 0.f;
	}

	return false;
}

bool USAbilityComponent::CheckCanActivate(USkill* TargetSkill)
{
	for (auto it : SkillSlots)
	{
		// 같은 스킬일 경우 패스
		if (TargetSkill->GetThisSkillType() == it.Key)
		{
			continue;
		}

		// 다른 스킬이 발동 중이면 캔슬하지 못한다.
		if (IsSkillActivated(it.Key))
		{
			return false;
		}
	}
	return true;
}

bool USAbilityComponent::IsSkillActivated(ESkillType SkillType)
{
	USkill* TargetSkill = GetTargetSkill(SkillType);
	if (TargetSkill != nullptr && CachedPawn)
	{
		if (TargetSkill->IsActivated(CachedPawn))
		{
			return true;
		}
	}

	return false;
}

void USAbilityComponent::SetSkillCost(USkill* TargetSkill)
{
	USAttributeComponent* AttributeComp = CachedPawn->GetAttributeComp();
	if (TargetSkill && AttributeComp)
	{
		// 마나
		AttributeComp->AddCurrentMana(-TargetSkill->GetManaCost());

		// 스킬 지속 시간
		TargetSkill->SetActivate(CachedPawn);

		// 쿨다운
		TargetSkill->SetCooldown(CachedPawn);
	}
}

USkill* USAbilityComponent::GetTargetSkill(ESkillType eSkillType)
{
	USkill** TargetSkill = SkillSlots.Find(eSkillType);
	if (TargetSkill != nullptr)
	{
		return *TargetSkill;
	}

	return nullptr;
}

USkill* USAbilityComponent::GetCurrentSkill()
{
	USkill** TargetSkill = SkillSlots.Find(CurrentSkillType);
	if (TargetSkill != nullptr)
	{
		return *TargetSkill;
	}

	return nullptr;
}
