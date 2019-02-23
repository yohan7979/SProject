// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "SCharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USAnimationHandler;
class USAttributeComponent;


UENUM()
enum class EWeaponCollisionType
{
	EWCT_Left,
	EWCT_Right,
	EWCT_ETC,
	EWCT_MAX
};

UENUM()
enum class EAnimMontageType
{
	EAMT_NormalAttack_A,
	EAMT_NormalAttack_B,
	EAMT_NormalAttack_C,
	EAMT_NormalAttack_D,
	EAMT_MAX
};

UCLASS()
class SPROJECT_API ASCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestDealDamage(AActor* OtherActor, float BaseDamage);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoSpecialAction(EAnimMontageType eType);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastDoSpecialAction(EAnimMontageType eType);
	virtual void DoSpeicalAction(EAnimMontageType eType);

public:
	virtual void Tick(float DeltaTime) override;

	void Move(const FVector& Direction, float fValue);
	void BeginAttack();
	void EndAttack();
	virtual void DoAttack();

	void SetWeaponCollision(EWeaponCollisionType eType, bool bEnable);

	UFUNCTION()
	void OnHealthChanged(float CurrentHealth, float DamageAmount, AActor* DamageCauser, AController* InstigatorController);
	
protected:
	UPROPERTY(EditAnywhere)
	UCameraComponent*		CameraComp;

	UPROPERTY(EditAnywhere)
	USpringArmComponent*	SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	USAnimationHandler*		AnimationHandler;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent*		LeftWeaponCollComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent*		RightWeaponCollComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USAttributeComponent*	AttributeComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackCooldown;
	FTimerHandle Timer_AttackCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LastAttackTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ComboCountKeepingTime;
	int8 ComboCount;
};
