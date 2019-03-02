// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerController.h"
#include "SCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacter = Cast<ASCharacterBase>(GetCharacter());
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ASPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ASPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ASPlayerController::LookUp);

	InputComponent->BindAction("Attack", IE_Pressed, this, &ASPlayerController::BeginAttack);
	InputComponent->BindAction("Attack", IE_Released, this, &ASPlayerController::EndAttack);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ASPlayerController::Jump);
	InputComponent->BindAction("AbilityOne", IE_Pressed, this, &ASPlayerController::AbilityOne);
}

void ASPlayerController::MoveForward(float fValue)
{
	float roll, pitch, yaw;
	UKismetMathLibrary::BreakRotator(GetControlRotation(), roll, pitch, yaw);
	FRotator Rot = UKismetMathLibrary::MakeRotator(0.f, 0.f, yaw);
	FVector Direction = UKismetMathLibrary::GetForwardVector(Rot);

	if (CachedCharacter != nullptr)
	{
		CachedCharacter->Move(Direction, fValue);
	}
}

void ASPlayerController::MoveRight(float fValue)
{
	float roll, pitch, yaw;
	UKismetMathLibrary::BreakRotator(GetControlRotation(), roll, pitch, yaw);
	FRotator Rot = UKismetMathLibrary::MakeRotator(0.f, 0.f, yaw);
	FVector Direction = UKismetMathLibrary::GetRightVector(Rot);

	if (CachedCharacter != nullptr)
	{
		CachedCharacter->Move(Direction, fValue);
	}
}

void ASPlayerController::Turn(float fValue)
{
	AddYawInput(fValue);
}

void ASPlayerController::LookUp(float fValue)
{
	AddPitchInput(fValue);
}

void ASPlayerController::BeginAttack()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->BeginAttack();
	}
}

void ASPlayerController::EndAttack()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->EndAttack();
	}
}

void ASPlayerController::Jump()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->DoJump();
	}
}

void ASPlayerController::AbilityOne()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->ExecuteAbilityOne();
	}
}

void ASPlayerController::AbilityTwo()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->ExecuteAbilityTwo();
	}
}

void ASPlayerController::AbilityThree()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->ExecuteAbilityThree();
	}
}

void ASPlayerController::AbliityFour()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->ExecuteAbilityFour();
	}
}

void ASPlayerController::SetControlledPawn(ASCharacterBase* InPawn)
{
	CachedCharacter = InPawn;
}
