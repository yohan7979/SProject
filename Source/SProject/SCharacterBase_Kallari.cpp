// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacterBase_Kallari.h"
#include "SAttributeComponent.h"
#include "SAnimationHandler.h"
#include "Kismet/GameplayStatics.h"

ASCharacterBase_Kallari::ASCharacterBase_Kallari()
{
	LeftWeaponCollComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftWeaponCollComp"));
	LeftWeaponCollComp->SetupAttachment(GetMesh(), "WeaponAttachPointL");

	RightWeaponCollComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightWeaponCollComp"));
	RightWeaponCollComp->SetupAttachment(GetMesh(), "WeaponAttachPointR");
}

void ASCharacterBase_Kallari::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		if (LeftWeaponCollComp != nullptr)
			LeftWeaponCollComp->OnComponentBeginOverlap.AddDynamic(this, &ASCharacterBase_Kallari::OnLeftCollisionBeginOverlap);

		if (RightWeaponCollComp != nullptr)
			RightWeaponCollComp->OnComponentBeginOverlap.AddDynamic(this, &ASCharacterBase_Kallari::OnRightCollisionBeginOverlap);
	}
}

void ASCharacterBase_Kallari::NormalAttack()
{
	if (AnimationHandler != nullptr)
	{
		AnimationHandler->PlayAnimationMontage(EAMT_Melee);
	}
}

void ASCharacterBase_Kallari::OnLeftCollisionBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Role == ROLE_Authority && OtherActor != this)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 20.f, GetController(), this, nullptr);
		UE_LOG(LogTemp, Warning, TEXT("LeftCollisonOverlapped! ApplyDamage 20.f"));
	}
}

void ASCharacterBase_Kallari::OnRightCollisionBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Role == ROLE_Authority && OtherActor != this)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 20.f, GetController(), this, nullptr);
		UE_LOG(LogTemp, Warning, TEXT("RightCollisonOverlapped! ApplyDamage 20.f"));
	}	
}