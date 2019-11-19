// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCharacterBase.h"
#include "SCharacterBase_Kallari.generated.h"

class ASProjectile;
class UCurveFloat;
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
	virtual void Tick(float DeltaTime) override;
	virtual void NotifiedSkillFinished(ESkillType SkillType) override;
	virtual void NotifyAnimationPlayed(FName AnimName) override;
	virtual void Landed(const FHitResult& Hit) override;
	
	virtual void ExecuteAbilityOne(EAnimMontageType& eAnimType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityTwo(EAnimMontageType& eAnimType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityThree(EAnimMontageType& eAnimType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityFour(EAnimMontageType& eAnimType, FName SectionName = NAME_None);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCreateDagger();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBoostSimluation();
	void ThrowDagger();
	void ReadyToDaggerThrow();
	void EndThrowDagger();

	UFUNCTION()
	void OnMeleeCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnRoundCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void TimelineFloatReturn(float value);

	UFUNCTION()
	void TimelineFinished();

	virtual void AddPreloadContent(FPreloadContentContainer& Collector, bool bIsDedicateServer);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASProjectile> DaggerProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	float DaggerRange;
	FName DaggerSocketName;
	FTimerHandle TimerHandle_DaggerThrow;

	UPROPERTY(EditDefaultsOnly, Category = "Timeline")
	UCurveFloat* CurveFloat;
	float StartFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Timeline")
	float EndFOV;

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* TimelineComp;
	FOnTimelineFloat OnTimelineFloat_Dagger;
	FOnTimelineEvent OnTimelineEvent_Dagger;

	UPROPERTY(EditDefaultsOnly)
	float BoostImpulseIntensity;

	UPROPERTY(EditDefaultsOnly)
	float BoostDirectionDegree;
};
