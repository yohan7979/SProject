// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;

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

protected:
	UPROPERTY(EditAnywhere)
	UCameraComponent*		CameraComp;
	UPROPERTY(EditAnywhere)
	USpringArmComponent*	SpringArmComp;

};
