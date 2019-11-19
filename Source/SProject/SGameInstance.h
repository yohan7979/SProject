// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "SGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPROJECT_API USGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()
	
public:
	//~ Begin UGameInstance interface
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void PreloadContentForURL(FURL InURL);
	//~ End UGameInstance  interface

private:
	class USGameSingleton* GameSingleton;
};
