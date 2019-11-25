// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"
#include "SGameSingleton.h"

USGameInstance::USGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameSingleton = nullptr;
}

void USGameInstance::Init()
{
	Super::Init();

	GameSingleton = USGameSingleton::Get();
	if (GameSingleton)
	{
		GameSingleton->Init(this);
	}
}

void USGameInstance::Shutdown()
{
	Super::Shutdown();

	if (GameSingleton)
	{
		GameSingleton->Shutdown();
	}

	GameSingleton = nullptr;
}

void USGameInstance::PreloadContentForURL(FURL InURL)
{
	Super::PreloadContentForURL(InURL);

	if (GameSingleton)
	{
		GameSingleton->PreloadContentForURL(InURL);
	}
}
