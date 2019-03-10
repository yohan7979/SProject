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

bool USAbilityComponent::ExecuteSkill(ESkillType SkillType)
{
	USkill** TargetSkill = SkillSlots.Find(SkillType);
	if (TargetSkill && CachedPawn)
	{
		if (CheckConditions(*TargetSkill) == true)
		{
			// �ߵ� ���� ������, �ڽ�Ʈ ��� ����
			SetSkillCost(*TargetSkill);
			return true;
		}
	}
	return false;
}

bool USAbilityComponent::CheckConditions(USkill* TargetSkill)
{
	// ��ٿ� �˻�
	if (!CheckCooldown(TargetSkill))
		return false;

	// ���� �ڽ�Ʈ �˻�
	if (!CheckManaCost(TargetSkill))
		return false;

	// Ÿ ��ų �ߵ� ������ �˻�
	if (!CheckCanActivate())
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

bool USAbilityComponent::CheckCanActivate()
{
	for (auto it : SkillSlots)
	{
		// �ٸ� ��ų�� �ߵ� ���̸� ĵ������ ���Ѵ�.
		if (IsSkillActivated(it.Key))
		{
			return false;
		}
	}
	return true;
}

bool USAbilityComponent::IsSkillActivated(ESkillType SkillType)
{
	if (SkillType == CurrentSkillType)
	{
		USkill* TargetSkill = GetCurrentSkill();
		if (TargetSkill != nullptr && CachedPawn)
		{
			if (TargetSkill->IsActivated(CachedPawn))
			{
				return true;
			}
		}
	}

	return false;
}

void USAbilityComponent::SetSkillCost(USkill* TargetSkill)
{
	USAttributeComponent* AttributeComp = CachedPawn->GetAttributeComp();
	if (TargetSkill && AttributeComp)
	{
		// ��ٿ�
		TargetSkill->SetCooldown(CachedPawn);

		// ����
		AttributeComp->AddCurrentMana(-TargetSkill->GetManaCost());

		// ��ų ���� �ð�
		TargetSkill->SetActivate(CachedPawn);
	}
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
