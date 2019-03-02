// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAbilityComponent.generated.h"

/** 
	캐릭터의 스킬을 담당하는 컴포넌트 
	스킬 컴포넌트를 관리하는 컨테이너를 가진다.
	캐릭터는 이 컴포넌트를 통해 등록된 스킬을 발동시킬 수 있다.
**/
UENUM()
enum class EAbilitySlotType
{
	EAST_None,
	EAST_One,
	EAST_Two,
	EAST_Three,
	EAST_Four
};

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
	void AddSkillSlot(EAbilitySlotType SlotType, USkill* NewSkill);
	void RemoveSkillSlot(EAbilitySlotType SlotType);

	bool ExecuteSkill(EAbilitySlotType SlotType);

	bool CheckConditions(USkill* TargetSkill);
	bool CheckCooldown(USkill* TargetSkill);
	bool CheckManaCost(USkill* TargetSkill);

	void UpdateProperties(USkill* TargetSkill);

public:	
	UPROPERTY(EditDefaultsOnly)
	TMap<EAbilitySlotType, TSubclassOf<USkill>> SkillSlotClasses;
	TMap<EAbilitySlotType, USkill*> SkillSlots;

private:
	ASCharacterBase* CachedPawn;
};
