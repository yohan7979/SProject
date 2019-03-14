// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerController.h"
#include "SCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "STypes.h"

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
	InputComponent->BindAction("SubAttack", IE_Pressed, this, &ASPlayerController::BeginSubAttack);
	InputComponent->BindAction("SubAttack", IE_Released, this, &ASPlayerController::EndSubAttack);
	InputComponent->BindAction("AbilityOne", IE_Pressed, this, &ASPlayerController::AbilityOne);
	InputComponent->BindAction("AbilityTwo", IE_Pressed, this, &ASPlayerController::AbilityTwo);

	InputComponent->BindAction<FJumpInputDelegate>("Jump", IE_Pressed, this, &ASPlayerController::Jump, true);
	InputComponent->BindAction<FJumpInputDelegate>("Jump", IE_Released, this, &ASPlayerController::Jump, false);

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

void ASPlayerController::BeginSubAttack()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->BeginSubAttack();
	}
}

void ASPlayerController::EndSubAttack()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->EndSubAttack();
	}
}

void ASPlayerController::Jump(bool bPressed)
{
	if (CachedCharacter != nullptr)
	{
		if (bPressed)
		{
			CachedCharacter->DoJump();
		}
		else
		{
			CachedCharacter->StopJump();
		}
	}
}

void ASPlayerController::AbilityOne()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->ExecuteAbility(EAnimMontageType::EAMT_Ability_One, ESkillType::EAST_One);
	}
}

void ASPlayerController::AbilityTwo()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->ExecuteAbility(EAnimMontageType::EAMT_Ability_Two, ESkillType::EAST_Two);
	}
}

void ASPlayerController::AbilityThree()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->ExecuteAbility(EAnimMontageType::EAMT_Ability_Three, ESkillType::EAST_Three);
	}
}

void ASPlayerController::AbliityFour()
{
	if (CachedCharacter != nullptr)
	{
		CachedCharacter->ExecuteAbility(EAnimMontageType::EAMT_Ability_Four, ESkillType::EAST_Four);
	}
}

void ASPlayerController::SetControlledPawn(ASCharacterBase* InPawn)
{
	CachedCharacter = InPawn;
}
