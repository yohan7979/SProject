// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAnimationHandler.generated.h"

class UAnimMontage;
class ASCharacterBase;

UENUM()
enum class EAnimMontageType
{
	EAMT_None,
	EAMT_NormalAttack_A,
	EAMT_NormalAttack_B,
	EAMT_NormalAttack_C,
	EAMT_NormalAttack_D,
	EAMT_Ability_One,
	EAMT_Death_A,
	EAMT_Death_B
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPROJECT_API USAnimationHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	USAnimationHandler();

protected:
	virtual void BeginPlay() override;

public:	
	void PlayAnimationMontage(EAnimMontageType eAnimType);

private:
	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	TMap<EAnimMontageType, UAnimMontage*> AnimMontageList;

	ASCharacterBase* CachedCharacter;
};
