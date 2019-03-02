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
		EAbilitySlotType SkillType = it.Key;
		if (it.Value != nullptr)
		{
			USkill* SkillSlot = it.Value->GetDefaultObject<USkill>();
			AddSkillSlot(SkillType, SkillSlot);
		}
	}
	
}

void USAbilityComponent::AddSkillSlot(EAbilitySlotType SlotType, USkill* NewSkill)
{
	RemoveSkillSlot(SlotType);

	SkillSlots.Add(SlotType, NewSkill);
}

void USAbilityComponent::RemoveSkillSlot(EAbilitySlotType SlotType)
{
	USkill** TargetSkill = SkillSlots.Find(SlotType);

	if (TargetSkill)
	{
		SkillSlots.Remove(SlotType);
	}
}

bool USAbilityComponent::ExecuteSkill(EAbilitySlotType SlotType)
{
	USkill** TargetSkill = SkillSlots.Find(SlotType);
	if (TargetSkill && CachedPawn)
	{
		if (CheckConditions(*TargetSkill) == true)
		{
			// �ߵ� ���� ������, �ڽ�Ʈ ��� ����
			UpdateProperties(*TargetSkill);
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
		return AttributeComp->GetCurrentMana() - TargetSkill->GetManaCost() > 0.f;
	}

	return false;
}

void USAbilityComponent::UpdateProperties(USkill* TargetSkill)
{
	USAttributeComponent* AttributeComp = CachedPawn->GetAttributeComp();
	if (TargetSkill && AttributeComp)
	{
		TargetSkill->SetCooldown(CachedPawn);
		AttributeComp->AddCurrentMana(-TargetSkill->GetManaCost());
	}
}
