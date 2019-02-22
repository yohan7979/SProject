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
	virtual void NormalAttack() override;

	UFUNCTION()
	void OnLeftCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnRightCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
