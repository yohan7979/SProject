// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameSingleton.h"
#include "Engine/GameInstance.h"
#include "SContentPreloader.h"

USGameSingleton::USGameSingleton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void USGameSingleton::Init(class USGameInstance* InGameInstance)
{
	GameInstance = InGameInstance;

	ContentPreloader = NewObject<USContentPreloader>(this, USContentPreloader::StaticClass());
	if (ContentPreloader)
	{
		ContentPreloader->Init(GameInstance, StreambleManager);
	}
}

void USGameSingleton::Shutdown()
{
	GameInstance = nullptr;

	if (ContentPreloader)
	{
		ContentPreloader->Shutdown();
		ContentPreloader = nullptr;
	}
}

void USGameSingleton::PreloadContentForURL(FURL InURL)
{
	if (ContentPreloader)
	{
		ContentPreloader->PreloadContentForURL(InURL);
	}
}
