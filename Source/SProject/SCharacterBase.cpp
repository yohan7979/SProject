// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "SAnimationHandler.h"

ASCharacterBase::ASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	AnimationHandler = CreateDefaultSubobject<USAnimationHandler>(TEXT("AnimationHandler"));
}

void ASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCharacterBase::Move(const FVector& Direction, float fValue)
{
	AddMovementInput(Direction, fValue);
}

void ASCharacterBase::NormalAttack()
{

}

void ASCharacterBase::SetWeaponCollision(EWeaponCollisionType eType, bool bEnable)
{
	UCapsuleComponent* TargetCollComp = nullptr;

	switch (eType)
	{
	case EWeaponCollisionType::EWCT_Left:
		TargetCollComp = LeftWeaponCollComp;
		break;
	case EWeaponCollisionType::EWCT_Right:
		TargetCollComp = RightWeaponCollComp;
		break;
	default:;
		return;
	}

	if (TargetCollComp != nullptr)
	{
		TargetCollComp->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

