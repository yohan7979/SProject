// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "Skill.generated.h"

class USAttributeComponent;
class ASCharacterBase;

UCLASS( ClassGroup=(Custom), Blueprintable)
class SPROJECT_API USkill : public UObject
{
	GENERATED_BODY()

public:	
	USkill();

public:
	bool InCooldown(ASCharacterBase* Owner);
	void SetCooldown(ASCharacterBase* Owner);

public:
	float GetCooldown() const { return Cooldown; }
	float GetDamage() const { return Damage; }
	float GetManaCost() const { return ManaCost; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Cooldown;
	FTimerHandle TimerHandle_Cooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ManaCost;
};
