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

	// 2�� ��ų ĳ���� �� ���� ��� �� Skill Confirm
	if (bCasting)
	{
		ExecuteAbility(EAnimMontageType::Ability_Two, ESkillType::Two, "End");
	}
	// �� �ܴ� �Ϲ� ���� ó��
	else
	{
		Super::DoAttack();

		ThrowStar();
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

			// 1�� ���� : �غ�
			if (bCasting)
			{
				StartCastingMeteor();
			}
			// 2�� ���� : ĳ���� OR ĵ��
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

void ASCharacterBase_Phase::ThrowStar()
{
	// ������Ÿ�� - Spawn On Server
	FVector AimDir = GetControlRotation().Vector();
	const FName& TargetSocketName = (ComboCount % 2 == 0) ? LeftHandSocketName : RightHandSocketName;
	const FVector& SocketLocation = GetMesh()->GetSocketLocation(TargetSocketName);

	FHitResult Hit(1.f);
	if (DoHitScanTrace(Hit))
	{
		AimDir = Hit.ImpactPoint - Hit.TraceStart;
		AimDir.Normalize();
	}

	//FVector SocketLocation = GetMesh()->GetSocketLocation(DaggerSocketName);
	CreateProjectile(BaseProjectileClass, SocketLocation, AimDir.Rotation());
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