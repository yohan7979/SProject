
#include "Skill.h"
#include "SAttributeComponent.h"
#include "SCharacterBase.h"
#include "TimerManager.h"

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

bool USkill::IsActivated(ASCharacterBase* Owner) const
{
	if (Owner)
	{
		return Owner->GetWorldTimerManager().IsTimerActive(TimerHandle_Duration);
	}
	return false;
}

void USkill::SetActivate(ASCharacterBase* Owner)
{
	if (Owner)
	{
		// InRate가 0.f인 경우 SetTimer가 무시됨.
		FTimerDelegate TimerDelegate_Duration = FTimerDelegate::CreateUObject(this, &USkill::NotifyFinishToOwner, Owner);
		Owner->GetWorldTimerManager().SetTimer(TimerHandle_Duration, TimerDelegate_Duration, Duration, false);
	}
}

void USkill::ClearActivate(ASCharacterBase* Owner)
{
	if (Owner)
	{
		Owner->GetWorldTimerManager().ClearTimer(TimerHandle_Duration);
	}
}

void USkill::NotifyFinishToOwner(ASCharacterBase* Owner)
{
	if (Owner)
	{
		Owner->NotifiedSkillFinished(ThisSkillType);
	}
}
