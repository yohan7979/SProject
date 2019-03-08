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
	void PlayAnimationMontage(EAnimMontageType eAnimType);

private:
	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	TMap<EAnimMontageType, UAnimMontage*> AnimMontageList;

	ASCharacterBase* CachedCharacter;
};
