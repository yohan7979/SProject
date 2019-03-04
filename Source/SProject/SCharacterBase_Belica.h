// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCharacterBase.h"
#include "SCharacterBase_Belica.generated.h"

/**
 * 
 */
UCLASS()
class SPROJECT_API ASCharacterBase_Belica : public ASCharacterBase
{
	GENERATED_BODY()
	
public:
	ASCharacterBase_Belica();

protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAttack() override;
	virtual bool ExecuteAbilityOne() override;
	virtual bool ExecuteAbilityTwo() override;
	virtual bool ExecuteAbilityThree() override;
	virtual bool ExecuteAbilityFour() override;
};
