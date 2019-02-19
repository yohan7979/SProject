// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class ASCharacterBase;
/**
 * 
 */

UCLASS()
class SPROJECT_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	void MoveForward(float fValue);
	void MoveRight(float fValue);
	void Turn(float fValue);
	void LookUp(float fValue);

	
protected:
	ASCharacterBase* CachedCharacter;
};
