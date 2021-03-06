// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STypes.h"
#include "SAnimationHandler.generated.h"

class UAnimMontage;
class ASCharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPROJECT_API USAnimationHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	USAnimationHandler();

protected:
	virtual void BeginPlay() override;

public:	
	void PlayAnimationMontage(EAnimMontageType eAnimType, FName SectionName = NAME_None, bool bReverse = false);
	void StopAnimationMontage(EAnimMontageType eAnimType);

	bool IsMontangePlaying(EAnimMontageType eAnimType);

private:
	UPROPERTY(EditDefaultsOnly, Category="AnimMontage", meta=(AllowPrivateAccess="true"))
	TMap<EAnimMontageType, UAnimMontage*> AnimMontageList;

	ASCharacterBase* CachedCharacter;
};
