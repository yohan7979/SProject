// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STypes.h"
#include "SAbilityComponent.generated.h"

/** 
	ĳ������ ��ų�� ����ϴ� ������Ʈ 
	��ų ������Ʈ�� �����ϴ� �����̳ʸ� ������.
	ĳ���ʹ� �� ������Ʈ�� ���� ��ϵ� ��ų�� �ߵ���ų �� �ִ�.
**/

class USkill;
class ASCharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPROJECT_API USAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USAbilityComponent();

protected:
	virtual void BeginPlay() override;

public:
	void AddSkillSlot(ESkillType SkillType, USkill* NewSkill);
	void RemoveSkillSlot(ESkillType SkillType);

	bool CanExecute(ESkillType SkillType);
	void ExecuteSkill(ESkillType SkillType);
	void CancelSkill(ESkillType SkillType);

	bool CheckConditions(USkill* TargetSkill);
	bool CheckCooldown(USkill* TargetSkill);
	bool CheckManaCost(USkill* TargetSkill);
	bool CheckCanActivate();
	bool IsSkillActivated(ESkillType SkillType);

	void SetSkillCost(USkill* TargetSkill);
	
	USkill* GetTargetSkill(ESkillType eSkillType);
	USkill* GetCurrentSkill();
	void SetCurrentSkillType(ESkillType eSkillType) { CurrentSkillType = eSkillType; }

public:	
	UPROPERTY(EditDefaultsOnly)
	TMap<ESkillType, TSubclassOf<USkill>> SkillSlotClasses;
	UPROPERTY() // Reflection
	TMap<ESkillType, USkill*> SkillSlots;

private:
	ASCharacterBase* CachedPawn;
	ESkillType CurrentSkillType;
};
