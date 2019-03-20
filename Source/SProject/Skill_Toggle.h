// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "Skill_Toggle.generated.h"

/**
 * 
 */
UCLASS()
class SPROJECT_API USkill_Toggle : public USkill
{
	GENERATED_BODY()

public:
	virtual bool InCooldown(ASCharacterBase* Owner) override;
	virtual void SetCooldown(ASCharacterBase* Owner) override;

	virtual void SetActivate(ASCharacterBase* Owner) override;
	virtual void ClearActivate(ASCharacterBase* Owner) override;
	virtual void NotifyFinishToOwner(ASCharacterBase* Owner) override;

	virtual float GetCooldown() const override;
	virtual float GetManaCost() const override;

public:
	bool IsCasting() const { return bCasting; }

private:
	UPROPERTY(EditDefaultsOnly)
	float SecondCooldown;
	FTimerHandle TimerHandle_SecondCooldown;

	UPROPERTY(EditDefaultsOnly)
	float SecondManaCost;

	bool bCasting;
};
