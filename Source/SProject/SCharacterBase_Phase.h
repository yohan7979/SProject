// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCharacterBase.h"
#include "SCharacterBase_Phase.generated.h"

class ASTargetActor;
class ASProjectile;
/**
 * 
 */
UCLASS()
class SPROJECT_API ASCharacterBase_Phase : public ASCharacterBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	ASCharacterBase_Phase();

	virtual void Tick(float DeltaTime) override;

	virtual void DoAttack() override;

	virtual void ExecuteAbilityOne(EAnimMontageType& eAnimType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityTwo(EAnimMontageType& eAnimType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityThree(EAnimMontageType& eAnimType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityFour(EAnimMontageType& eAnimType, FName SectionName = NAME_None);

	void StartCastingMeteor();
	void EndCastingMeteor();
	void DropMeteor();

	void ThrowStar();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDropMeteor(const FVector& TargetLocation);

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASProjectile> BaseProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	FName LeftHandSocketName;

	UPROPERTY(EditDefaultsOnly)
	FName RightHandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Meteor")
	TSubclassOf<ASTargetActor> TargetActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Meteor")
	TSubclassOf<ASProjectile> MeteorProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Meteor")
	float DropStartHeight;

private:
	UPROPERTY()
	ASTargetActor* TargetActor;
};
