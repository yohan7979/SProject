// Fill out your copyright notice in the Description page of Project Settings.


#include "SContentPreloader.h"
#include "SGameInstance.h"
#include "GameMapsSettings.h"
#include "GameFramework/GameModeBase.h"

USContentPreloader::USContentPreloader(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameModeClassName.Empty();
}

void USContentPreloader::Init(class USGameInstance* InGameInstance, FStreamableManager& InStreamableManager)
{
	GameInstance = InGameInstance;
	StreamableManager = &InStreamableManager;

	if (StreamableManager)
	{
		FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &USContentPreloader::OnPreloadMap);
		FWorldDelegates::OnWorldInitializedActors.AddUObject(this, &USContentPreloader::OnWorldInitializedActors);
	}
}

void USContentPreloader::Shutdown()
{
	GameInstance = nullptr;

	if (StreamableManager)
	{
		FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
		FWorldDelegates::OnWorldInitializedActors.RemoveAll(this);
		StreamableManager = nullptr;
	}
}

void USContentPreloader::PreloadContentForURL(FURL InURL)
{
	// 월드 초기화 전
}

void USContentPreloader::OnPreloadMap(const FString& MapName)
{
	// 새로운 맵 로드 시, 기존 Preload 모두 제거
	for (FPreloadInfo PreloadInfo : PreloadContainer.PreloadList)
	{
		if (StreamableManager)
		{
			StreamableManager->Unload(PreloadInfo.Path);
		}
	}

	GameModeClassName.Empty();
	PreloadedContents.Empty();
	PreloadContainer.PreloadList.Empty();
}

void USContentPreloader::OnWorldInitializedActors(const struct UWorld::FActorsInitializedParams& ActorInitParams)
{
	UWorld* World = ActorInitParams.World;
	
	if (World == nullptr)
	{
		return;
	}

	TSubclassOf<AGameModeBase> GameModeClass = nullptr;

	if (!GameModeClassName.IsEmpty())
	{
		GameModeClass = LoadClass<AGameModeBase>(nullptr, *GameModeClassName);
	}

	if (GameModeClassName.IsEmpty())
	{
		GameModeClassName = UGameMapsSettings::GetGlobalDefaultGameMode();
		GameModeClass = LoadClass<AGameModeBase>(nullptr, *GameModeClassName);
	}
	
	if (GameModeClass)
	{
		BeginPreloadByGameMode(GameModeClass);
	}
}

void USContentPreloader::BeginPreloadByGameMode(TSubclassOf<AGameModeBase> GameModeClass)
{
	AGameModeBase* DefaultGameMode = GameModeClass->GetDefaultObject<AGameModeBase>();
	if (DefaultGameMode)
	{
		CollectPreloadContents(DefaultGameMode->DefaultPawnClass->GetDefaultObject());
	}

	StartPreloadContents();
}

void USContentPreloader::CollectPreloadContents(UClass* InClass)
{
	if (InClass)
	{
		UObject* DefaultObject = InClass->GetDefaultObject();
		if (DefaultObject)
		{
			CollectPreloadContents(DefaultObject);
		}
	}
}

void USContentPreloader::CollectPreloadContents(UObject* InObject)
{
	if (InObject)
	{
		IPreloadContentInterface* TargetContent = Cast<IPreloadContentInterface>(InObject);
		if (TargetContent)
		{
			TargetContent->AddPreloadContent(PreloadContainer, IsRunningDedicatedServer());
		}
	}
}

void USContentPreloader::StartPreloadContents()
{
	for (const FPreloadInfo& PreloadInfo : PreloadContainer.PreloadList)
	{
		if (StreamableManager)
		{
			// 싱크 로드
			TSharedPtr<FStreamableHandle> Handle = StreamableManager->RequestSyncLoad(PreloadInfo.Path, true);

			if (Handle.IsValid())
			{
				UObject* LoadedObject = Handle->GetLoadedAsset();
				if (LoadedObject)
				{
					// GC되지 않게 레퍼런스 저장
					PreloadedContents.Add(LoadedObject);
				}

				if (PreloadInfo.bIsClass)
				{
					UClass* LoadedClass = Cast<UClass>(LoadedObject);
					if (LoadedClass)
					{
						PreloadedContents.Add(LoadedClass->GetDefaultObject());
					}
				}

				UE_LOG(LogTemp, Warning, TEXT("** Preload Contents : %s"), *GetNameSafe(LoadedObject));

				// 로드된 오브젝트 내부에서 찾는다.
				CollectPreloadContents(LoadedObject);
			}
		}
	}
}
