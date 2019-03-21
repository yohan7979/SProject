// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "STypes.h"
#include "Components/TimelineComponent.h"
#include "Skill.generated.h"

class USAttributeComponent;
class ASCharacterBase;
class UParticleSystem;
class UCameraShake;

UCLASS( ClassGroup=(Custom), Blueprintable)
class SPROJECT_API USkill : public UObject
{
	GENERATED_BODY()

public:	
	USkill();

public:
	ESkillType GetThisSkillType() const { return ThisSkillType; }
	void SetThisSkillType(ESkillType InSkillType) { ThisSkillType = InSkillType; }

	virtual bool InCooldown(ASCharacterBase* Owner);
	virtual void SetCooldown(ASCharacterBase* Owner);

	virtual bool IsActivated(ASCharacterBase* Owner) const;
	virtual void SetActivate(ASCharacterBase* Owner);
	virtual void ClearActivate(ASCharacterBase* Owner);
	virtual void NotifyFinishToOwner(ASCharacterBase* Owner);

	virtual float GetCooldown() const { return Cooldown; }
	virtual float GetDamage() const { return Damage; }
	virtual float GetManaCost() const { return ManaCost; }
	virtual float GetDuration() const { return Duration; }
	virtual UParticleSystem* GetParticleSystemWorld() const { return PS_HitWorld; }
	virtual UParticleSystem* GetParticleSystemPawn() const { return PS_HitPawn; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Cooldown;
	FTimerHandle TimerHandle_Cooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ManaCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Duration;
	FTimerHandle TimerHandle_Duration;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* PS_HitWorld;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* PS_HitPawn;

private:
	ESkillType ThisSkillType;
};
