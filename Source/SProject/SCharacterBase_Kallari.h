// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCharacterBase.h"
#include "SCharacterBase_Kallari.generated.h"

/**
 * 
 */
UCLASS()
class SPROJECT_API ASCharacterBase_Kallari : public ASCharacterBase
{
	GENERATED_BODY()
	
public:
	ASCharacterBase_Kallari();

	virtual void NormalAttack() override;
};
