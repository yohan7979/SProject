// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PreloadContentInterface.generated.h"

struct FPreloadInfo
{
	FSoftObjectPath Path;
	bool bIsClass;

	FPreloadInfo(const FSoftObjectPath& InPath, bool bInIsClass)
		: Path(InPath)
		, bIsClass(bInIsClass)
	{}

	bool operator==(const FPreloadInfo& Other)
	{
		return Path == Other.Path && bIsClass == Other.bIsClass;
	}

	friend bool operator==(const FPreloadInfo& lhs, const FPreloadInfo& rhs)
	{
		return lhs.Path == rhs.Path && lhs.bIsClass == rhs.bIsClass;
	}
};

struct FPreloadContentContainer
{
	template<class TClass = UObject>
	void Add(TSoftObjectPtr<TClass> SoftObjectPtr)
	{
		const FSoftObjectPath& SoftObjectPath = SoftObjectPtr.ToSoftObjectPath();
		if (SoftObjectPath.IsValid())
		{
			FPreloadInfo PreloadInfo(SoftObjectPath, false);
			PreloadList.AddUnique(PreloadInfo);
		}
	}

	template<class TClass = UObject>
	void Add(TSoftClassPtr<TClass> SoftClassPtr)
	{
		const FSoftObjectPath& SoftObjectPath = SoftClassPtr.ToSoftObjectPath();
		if (SoftObjectPath.IsValid())
		{
			FPreloadInfo PreloadInfo(SoftObjectPath, true);
			PreloadList.AddUnique(PreloadInfo);
		}
	}

	FPreloadContentContainer() 
		: PreloadList()
	{
	}

	virtual ~FPreloadContentContainer()
	{
		PreloadList.Empty();
	}

	TArray<FPreloadInfo> PreloadList;
};


UINTERFACE()
class UPreloadContentInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SPROJECT_API IPreloadContentInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual void AddPreloadContent(FPreloadContentContainer& Collector, bool bIsDedicateServer) = 0;
};
