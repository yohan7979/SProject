// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacterBase_Phase.h"
#include "SAbilityComponent.h"
#include "Skill_Toggle.h"
#include "STargetActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "SProjectile.h"
#include "SAnimationHandler.h"


ASCharacterBase_Phase::ASCharacterBase_Phase()
{
	PrimaryActorTick.bCanEverTick = true;

	DropStartHeight = 200.f;
	LeftHandSocketName = FName(TEXT("hand_l"));
	RightHandSocketName = FName(TEXT("hand_r"));
	EnergyLanceDamageFrequency = 0.2f;
}

void ASCharacterBase_Phase::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacterBase_Phase::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
}

void ASCharacterBase_Phase::DoAttack()
{
	bool bCasting = false;
	if (AbilityComp)
	{
		USkill_Toggle* ToggleSkill = Cast<USkill_Toggle>(AbilityComp->GetTargetSkill(ESkillType::Two));
		if (ToggleSkill)
		{
			bCasting = ToggleSkill->IsCasting();
		}
	}

	// 2번 스킬 캐스팅 중 공격 명령 시 Skill Confirm
	if (bCasting)
	{
		ExecuteAbility(EAnimMontageType::Ability_Two, ESkillType::Two, "End");
	}
	// 그 외는 일반 공격 처리
	else
	{
		Super::DoAttack();

		ThrowStar();
	}
}

void ASCharacterBase_Phase::ExecuteAbilityOne(EAnimMontageType & eAnimType, FName SectionName)
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	if (!TimerManager.IsTimerActive(TimerHandle_EnergyLanceDamage))
	{
		TimerManager.SetTimer(TimerHandle_EnergyLanceDamage, this, &ASCharacterBase_Phase::HitScanTraceForEnegryLance, EnergyLanceDamageFrequency, true, 0.82f);
	}

	bLanceCasting = true;
}

void ASCharacterBase_Phase::ExecuteAbilityTwo(EAnimMontageType & eAnimType, FName SectionName)
{
	bool bCasting = false;
	if (AbilityComp)
	{
		USkill_Toggle* ToggleSkill = Cast<USkill_Toggle>(AbilityComp->GetTargetSkill(ESkillType::Two));
		if (ToggleSkill)
		{
			bCasting = ToggleSkill->IsCasting();

			// 1차 시전 : 준비
			if (bCasting)
			{
				StartCastingMeteor();
			}
			// 2차 시전 : 캐스팅 OR 캔슬
			else
			{
				if (SectionName == "End")
				{
					DropMeteor();
				}
				else
				{
					EndCastingMeteor();
					eAnimType = EAnimMontageType::None;
				}
			}
		}
	}
}

void ASCharacterBase_Phase::ExecuteAbilityThree(EAnimMontageType & eAnimType, FName SectionName)
{
}

void ASCharacterBase_Phase::ExecuteAbilityFour(EAnimMontageType & eAnimType, FName SectionName)
{
}

void ASCharacterBase_Phase::NotifiedSkillFinished(ESkillType SkillType)
{
	Super::NotifiedSkillFinished(SkillType);

	FTimerManager& TimerManager = GetWorldTimerManager();

	switch (SkillType)
	{
	case ESkillType::One:
		if (TimerManager.IsTimerActive(TimerHandle_EnergyLanceDamage))
		{
			TimerManager.ClearTimer(TimerHandle_EnergyLanceDamage);
		}
		bLanceCasting = false;
		break;
	default:
		break;
	}


}

void ASCharacterBase_Phase::StartCastingMeteor()
{
	if (TargetActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		TargetActor = GetWorld()->SpawnActor<ASTargetActor>(TargetActorClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		if (TargetActor)
		{
		}

		//bUseControllerRotationYaw = false;
	}
}

void ASCharacterBase_Phase::EndCastingMeteor()
{
	if (TargetActor)
	{
		TargetActor->Destroy();
	}

	//bUseControllerRotationYaw = true;
}

void ASCharacterBase_Phase::DropMeteor()
{
	if (TargetActor)
	{
		FVector TargetLocation = TargetActor->GetActorLocation();
		
		ServerDropMeteor(TargetLocation);
	}

	EndCastingMeteor();
}

void ASCharacterBase_Phase::ThrowStar()
{
	// 프로젝타일 - Spawn On Server
	FVector AimDir = GetControlRotation().Vector();
	const FName& TargetSocketName = (ComboCount % 2 == 0) ? LeftHandSocketName : RightHandSocketName;
	const FVector& SocketLocation = GetMesh()->GetSocketLocation(TargetSocketName);

	FHitResult Hit(1.f);
	if (DoHitScanTrace(Hit, SocketLocation))
	{
		AimDir = Hit.ImpactPoint - Hit.TraceStart;
		AimDir.Normalize();
	}

	CreateProjectile(BaseProjectileClass, SocketLocation, AimDir.Rotation());
}

void ASCharacterBase_Phase::HitScanTraceForEnegryLance()
{
	FHitResult Hit(ForceInit);
	if (DoHitScanTrace(Hit))
	{
		// Request damage to server
		if (Hit.Actor != nullptr)
		{
			ServerRequestDealDamage(Hit.GetActor(), GetSkillDamage());

			// play impact fx (on server, multicast)
			ServerPlayImpactEffect(Hit);
		}
	}
}

void ASCharacterBase_Phase::AddPreloadContent(FPreloadContentContainer& Collector, bool bIsDedicateServer)
{
	Super::AddPreloadContent(Collector, bIsDedicateServer);

	if (!bIsDedicateServer)
	{
		if (BaseProjectileClass)
		{
			ASProjectile* DefaultBaseProjectile = BaseProjectileClass->GetDefaultObject<ASProjectile>();
			if (DefaultBaseProjectile)
			{
				DefaultBaseProjectile->AddPreloadContent(Collector, bIsDedicateServer);
			}
		}

		if (MeteorProjectileClass)
		{
			ASProjectile* DefaultMeteorProjectile = MeteorProjectileClass->GetDefaultObject<ASProjectile>();
			if (DefaultMeteorProjectile)
			{
				DefaultMeteorProjectile->AddPreloadContent(Collector, bIsDedicateServer);
			}
		}
	}
}

void ASCharacterBase_Phase::ServerDropMeteor_Implementation(const FVector& TargetLocation)
{
	if (MeteorProjectileClass)
	{
		FVector DropLocation(TargetLocation.X, TargetLocation.Y, TargetLocation.Z + DropStartHeight);
		FVector ProjectileDir = FVector::UpVector * -1;

		CreateProjectile(MeteorProjectileClass, DropLocation, ProjectileDir.Rotation());
	}
}

bool ASCharacterBase_Phase::ServerDropMeteor_Validate(const FVector& TargetLocation)
{
	return true;
}