// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacterBase_Belica.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"

ASCharacterBase_Belica::ASCharacterBase_Belica()
{
	AttackCooldown = 0.2f;
	NormalDamage = 5.f;
	MaxComboCount = 1;
	MuzzleSocketName = "Muzzle_01";
}

void ASCharacterBase_Belica::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacterBase_Belica::DoAttack()
{
	Super::DoAttack();

	//클라이언트에서 LineTrace를 하고, Hit이 성공하면 서버에게 데미지 요청 + PlayImpact
	FVector StartLocation, EndLocation;
	FRotator AimDirection;
	
	StartLocation = GetPawnViewLocation();
	AimDirection = GetControlRotation();
	EndLocation = StartLocation + AimDirection.Vector() * 5000.f; // TODO: Make Range

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_GameTraceChannel1, CollisionParams))
	{
		// Request damage to server
		if (Hit.Actor != nullptr)
		{
			ServerRequestDealDamage(Hit.GetActor(), NormalDamage);

			// play impact fx (on server, multicast)
			ServerPlayImpactEffect(Hit);
		}
	}

	/** Muzzle 이펙트는 애님노티로 출력한다. */
}

void ASCharacterBase_Belica::BeginSubAttack()
{
	DoSpeicalAction(EAnimMontageType::EAMT_NormalAttack_B, ESkillType::EAST_Basic_B);
	if (Role < ROLE_Authority)
	{
		ServerDoSpecialAction(EAnimMontageType::EAMT_NormalAttack_B, ESkillType::EAST_Basic_B);
	}

	ServerZoom(true);
}

void ASCharacterBase_Belica::EndSubAttack()
{
	DoSpeicalAction(EAnimMontageType::EAMT_NormalAttack_B, ESkillType::EAST_Basic_B, "End");
	if (Role < ROLE_Authority)
	{
		ServerDoSpecialAction(EAnimMontageType::EAMT_NormalAttack_B, ESkillType::EAST_Basic_B, "End");
	}

	ServerZoom(false);
}

void ASCharacterBase_Belica::ExecuteAbilityOne(EAnimMontageType & eAnimType, FName SectionName)
{
}

void ASCharacterBase_Belica::ExecuteAbilityTwo(EAnimMontageType & eAnimType, FName SectionName)
{
}

void ASCharacterBase_Belica::ExecuteAbilityThree(EAnimMontageType & eAnimType, FName SectionName)
{
}

void ASCharacterBase_Belica::ExecuteAbilityFour(EAnimMontageType & eAnimType, FName SectionName)
{
}

void ASCharacterBase_Belica::ServerZoom_Implementation(bool InZoom)
{
	bZoom = InZoom;
}

bool ASCharacterBase_Belica::ServerZoom_Validate(bool InZoom)
{
	return true;
}

void ASCharacterBase_Belica::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacterBase_Belica, bZoom);
}
