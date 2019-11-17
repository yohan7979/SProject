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

	//Ŭ���̾�Ʈ���� LineTrace�� �ϰ�, Hit�� �����ϸ� �������� ������ ��û + PlayImpact
	FHitResult Hit;
	if (DoHitScanTrace(Hit))
	{
		// Request damage to server
		if (Hit.Actor != nullptr)
		{
			ServerRequestDealDamage(Hit.GetActor(), NormalDamage);

			// play impact fx (on server, multicast)
			ServerPlayImpactEffect(Hit);
		}
	}

	/** Muzzle ����Ʈ�� �ִԳ�Ƽ�� ����Ѵ�. */
}

void ASCharacterBase_Belica::BeginSubAttack()
{
	DoSpeicalAction(EAnimMontageType::NormalAttack_B, ESkillType::Basic_B);
	if (Role < ROLE_Authority)
	{
		ServerDoSpecialAction(EAnimMontageType::NormalAttack_B, ESkillType::Basic_B);
	}

	ServerZoom(true);
}

void ASCharacterBase_Belica::EndSubAttack()
{
	DoSpeicalAction(EAnimMontageType::NormalAttack_B, ESkillType::Basic_B, "End");
	if (Role < ROLE_Authority)
	{
		ServerDoSpecialAction(EAnimMontageType::NormalAttack_B, ESkillType::Basic_B, "End");
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
