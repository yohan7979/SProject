// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "STypes.h"
#include "Skill.generated.h"

class USAttributeComponent;
class ASCharacterBase;
class UParticleSystem;

UCLASS( ClassGroup=(Custom), Blueprintable)
class SPROJECT_API USkill : public UObject
{
	GENERATED_BODY()

public:	
	USkill();

public:
	void SetThisSkillType(ESkillType InSkillType) { ThisSkillType = InSkillType; }

	bool InCooldown(ASCharacterBase* Owner);
	void SetCooldown(ASCharacterBase* Owner);

	bool IsActivated(ASCharacterBase* Owner) const;
	void SetActivate(ASCharacterBase* Owner);
	void ClearActivate(ASCharacterBase* Owner);
	void NotifyFinishToOwner(ASCharacterBase* Owner);

public:
	float GetCooldown() const { return Cooldown; }
	float GetDamage() const { return Damage; }
	float GetManaCost() const { return ManaCost; }
	float GetDuration() const { return Duration; }
	UParticleSystem* GetParticleSystemWorld() const { return PS_HitWorld; }
	UParticleSystem* GetParticleSystemPawn() const { return PS_HitPawn; }

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
