// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCharacterBase.h"
#include "SCharacterBase_Belica.generated.h"

class UParticleSystem;
/**
 * 
 */
UCLASS()
class SPROJECT_API ASCharacterBase_Belica : public ASCharacterBase
{
	GENERATED_BODY()
	
public:
	ASCharacterBase_Belica();

protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAttack() override;
	virtual void BeginSubAttack() override;
	virtual void EndSubAttack() override;

	virtual void ExecuteAbilityOne(EAnimMontageType& eAnimType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityTwo(EAnimMontageType& eAnimType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityThree(EAnimMontageType& eAnimType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityFour(EAnimMontageType& eAnimType, FName SectionName = NAME_None);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerZoom(bool InZoom);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	FName MuzzleSocketName;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bZoom;
};
