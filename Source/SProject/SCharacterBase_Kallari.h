// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCharacterBase.h"
#include "SCharacterBase_Kallari.generated.h"

/**
 * 
 */
UCLASS()
class SPROJECT_API ASCharacterBase_Kallari : public ASCharacterBase
{
	GENERATED_BODY()


public:
	ASCharacterBase_Kallari();

protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAttack() override;
	virtual bool ExecuteAbility(EAnimMontageType eAnimType, ESkillType eSkillType) override;
	virtual void NotifiedSkillFinished(ESkillType SkillType) override;

	void ThrowDagger();

	UFUNCTION()
	void OnMeleeCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnRoundCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
