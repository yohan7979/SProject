// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SAnimNotify_PlayNoti.generated.h"

/**
 * 
 */
UCLASS()
class SPROJECT_API USAnimNotify_PlayNoti : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

protected:
	UPROPERTY(EditAnywhere)
	FName NotifyName;
};
