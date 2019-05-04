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
	}
}

void ASCharacterBase_Phase::ExecuteAbilityOne(EAnimMontageType & eAnimType, FName SectionName)
{
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

void ASCharacterBase_Phase::ServerDropMeteor_Implementation(const FVector& TargetLocation)
{
	if (MeteorProjectileClass)
	{
		FVector DropLocation(TargetLocation.X, TargetLocation.Y, TargetLocation.Z + DropStartHeight);
		FVector ProjectileDir = FVector::UpVector * -1;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASProjectile* Projectile = GetWorld()->SpawnActor<ASProjectile>(MeteorProjectileClass, DropLocation, ProjectileDir.Rotation(), SpawnParams);
		if (Projectile)
		{
			Projectile->SetProjectileDirection(ProjectileDir);
			Projectile->SetProjectileDamage(GetSkillDamage());
		}
	}
}

bool ASCharacterBase_Phase::ServerDropMeteor_Validate(const FVector& TargetLocation)
{
	return true;
}