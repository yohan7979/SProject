// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine.h"
#include "Engine/StreamableManager.h"
#include "SGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class SPROJECT_API USGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	static USGameSingleton* Get()
	{
		return CastChecked<USGameSingleton>(GEngine->GameSingleton);
	}

	virtual void Init(class USGameInstance* InGameInstance);
	virtual void Shutdown();
	virtual void PreloadContentForURL(FURL InURL);

	UPROPERTY(Transient)
	class USGameInstance* GameInstance;

	UPROPERTY(Transient)
	class USContentPreloader* ContentPreloader;

private:
	FStreamableManager StreambleManager;
};
