// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "SCharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USAnimationHandler;


UENUM()
enum class EWeaponCollisionType
{
	EWCT_Left,
	EWCT_Right,
	EWCT_ETC,
	EWCT_MAX
};

UCLASS()
class SPROJECT_API ASCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Move(const FVector& Direction, float fValue);
	virtual void NormalAttack();

	void SetWeaponCollision(EWeaponCollisionType eType, bool bEnable);

protected:
	UPROPERTY(EditAnywhere)
	UCameraComponent*		CameraComp;

	UPROPERTY(EditAnywhere)
	USpringArmComponent*	SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	USAnimationHandler*		AnimationHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent*		LeftWeaponCollComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent*		RightWeaponCollComp;
};
