// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAnimationHandler.generated.h"

class UAnimMontage;
class ASCharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPROJECT_API USAnimationHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAnimationHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PlayAnimationMontage(EAnimMontageType eAnimType);

private:
	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	UAnimMontage* AnimMontageList[EAMT_MAX];

	ASCharacterBase* CachedCharacter;
};
