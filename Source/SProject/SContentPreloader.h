// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PreloadContentInterface.h"
#include "Engine/StreamableManager.h"
#include "Engine/World.h"
#include "SContentPreloader.generated.h"

UCLASS()
class SPROJECT_API USContentPreloader : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	void Init(class USGameInstance* InGameInstance, FStreamableManager& InStreamableManager);
	void Shutdown();
	void PreloadContentForURL(FURL InURL);
	
	void OnPreloadMap(const FString& MapName);
	void OnWorldInitializedActors(const struct UWorld::FActorsInitializedParams& ActorInitParams);

	void BeginPreloadByGameMode(TSubclassOf<AGameModeBase> GameModeClass);
	void CollectPreloadContents(UClass* InClass);
	void CollectPreloadContents(UObject* InObject);
	void StartPreloadContents();

private:
	UPROPERTY(Transient)
	class USGameInstance* GameInstance;
	
	FStreamableManager* StreamableManager;
	FPreloadContentContainer PreloadContainer;
	
	FString GameModeClassName;

	UPROPERTY(Transient)
	TArray<UObject*> PreloadedContents;
};
