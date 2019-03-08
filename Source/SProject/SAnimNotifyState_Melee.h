// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "STypes.h"
#include "SAnimNotifyState_Melee.generated.h"

/**
 * 
 */
UCLASS()
class SPROJECT_API USAnimNotifyState_Melee : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="SelectedCollisionType")
	EWeaponCollisionType WeaponCollType;
};
