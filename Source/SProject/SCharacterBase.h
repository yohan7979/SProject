// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "STypes.h"
#include "SCharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USAttributeComponent;
class USAnimationHandler;
class USAbilityComponent;
class USphereComponent;
class UCapsuleComponent;
class USkill;

UCLASS()
class SPROJECT_API ASCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestDealDamage(AActor* OtherActor, float BaseDamage);

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerPlayImpactEffect(const FHitResult& HitResult);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayImpactEffect(const FHitResult& HitResult);
	virtual void PlayImpactEffect(const FHitResult& HitResult);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoSpecialAction(EAnimMontageType eAnimType, ESkillType eSkillType = ESkillType::None, FName SectionName = NAME_None);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDoSpecialAction(EAnimMontageType eAnimType, ESkillType eSkillType = ESkillType::None, FName SectionName = NAME_None);
	virtual void DoSpeicalAction(EAnimMontageType eAnimType, ESkillType eSkillType = ESkillType::None, FName SectionName = NAME_None);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetDied(bool isDie);
	
	UFUNCTION()
	virtual void OnRep_Died();

	virtual void Tick(float DeltaTime) override;
	USAttributeComponent* GetAttributeComp() const { return AttributeComp; }


	void Move(const FVector& Direction, float fValue);
	void BeginAttack();
	void EndAttack();
	virtual void BeginSubAttack();
	virtual void EndSubAttack();
	virtual void DoAttack();
	virtual void DoJump();
	virtual void StopJump();

	virtual bool ExecuteAbility(EAnimMontageType eAnimType, ESkillType eSkillType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityOne(EAnimMontageType& eAnimType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityTwo(EAnimMontageType& eAnimType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityThree(EAnimMontageType& eAnimType, FName SectionName = NAME_None);
	virtual void ExecuteAbilityFour(EAnimMontageType& eAnimType, FName SectionName = NAME_None);


	void SetWeaponCollision(EWeaponCollisionType eType, bool bEnable);
	void GetAnimMontageByComboCount(EAnimMontageType& eAnimType, ESkillType& eSkillType);
	void CalculateComboCount();
	void ResetComboCount();
	virtual void NotifiedSkillFinished(ESkillType SkillType);
	float GetSkillDamage() const;

	virtual void NotifyAnimationPlayed(FName AnimName);

	UFUNCTION()
	void OnHealthChanged(float CurrentHealth, float DamageAmount, AActor* DamageCauser, AController* InstigatorController);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent*		CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent*	SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	USAnimationHandler*		AnimationHandler;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent*		MeleeCollComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent*		RoundCollComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USAttributeComponent*	AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USAbilityComponent*		AbilityComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float NormalDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackCooldown;
	float LastAttackTime;
	FTimerHandle TimerHandle_AttackCooldown;

	UPROPERTY(EditDefaultsOnly)
	bool bRandomCombo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ComboCountKeepingTime;

	UPROPERTY(EditDefaultsOnly)
	int8 MaxComboCount;
	int8 ComboCount;
	FTimerHandle TimerHandle_Combo;

	UPROPERTY(ReplicatedUsing=OnRep_Died, BlueprintReadOnly)
	bool bDied;
};
