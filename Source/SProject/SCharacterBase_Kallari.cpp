// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacterBase_Kallari.h"
#include "SAttributeComponent.h"
#include "SAbilityComponent.h"
#include "Skill.h"
#include "SAnimationHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"

ASCharacterBase_Kallari::ASCharacterBase_Kallari()
{
	MeleeCollComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCollComp"));
	MeleeCollComp->SetupAttachment(RootComponent);

	RoundCollComp = CreateDefaultSubobject<USphereComponent>(TEXT("RoundCollComp"));
	RoundCollComp->SetupAttachment(RootComponent);

	AttackCooldown = 1.f;
	ComboCountKeepingTime = 1.5f;
	NormalDamage = 20.f;
	MaxComboCount = 3;
}

void ASCharacterBase_Kallari::BeginPlay()
{
	Super::BeginPlay();

	if (MeleeCollComp != nullptr)
		MeleeCollComp->OnComponentBeginOverlap.AddDynamic(this, &ASCharacterBase_Kallari::OnMeleeCollisionBeginOverlap);

	if (RoundCollComp != nullptr)
		RoundCollComp->OnComponentBeginOverlap.AddDynamic(this, &ASCharacterBase_Kallari::OnRoundCollisionBeginOverlap);
}

void ASCharacterBase_Kallari::DoAttack()
{
	if (AbilityComp && AbilityComp->IsSkillActivated(ESkillType::EAST_Two))
	{
		AbilityComp->GetCurrentSkill()->ClearActivate(this);
		ThrowDagger();
	}
	else
	{
		Super::DoAttack();
	}
}

bool ASCharacterBase_Kallari::ExecuteAbility(EAnimMontageType eAnimType, ESkillType eSkillType)
{
	return Super::ExecuteAbility(eAnimType, eSkillType);
}

void ASCharacterBase_Kallari::NotifiedSkillFinished(ESkillType SkillType)
{
	Super::NotifiedSkillFinished(SkillType);

	switch (SkillType)
	{
	case ESkillType::EAST_Two:
		DoSpeicalAction(EAnimMontageType::EAMT_CancelMotion);
		if (Role < ROLE_Authority)
		{
			ServerDoSpecialAction(EAnimMontageType::EAMT_CancelMotion);
		}
		break;
	default:
		break;
	}
}

void ASCharacterBase_Kallari::ThrowDagger()
{
	// 애니메이션
	DoSpeicalAction(EAnimMontageType::EAMT_Ability_Two, ESkillType::EAST_Two, "End");
	if (Role < ROLE_Authority)
	{
		ServerDoSpecialAction(EAnimMontageType::EAMT_Ability_Two, ESkillType::EAST_Two, "End");
	}

	// 프로젝타일
}

void ASCharacterBase_Kallari::OnMeleeCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsLocallyControlled() && OtherActor != this)
	{
		const USkill* TargetSkill = AbilityComp->GetCurrentSkill();
		float RealDamage = TargetSkill ? TargetSkill->GetDamage() : NormalDamage;

		ServerRequestDealDamage(OtherActor, RealDamage);

		FHitResult HitResult(OtherActor, OtherComp, OtherActor->GetActorLocation(), (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal());
		ServerPlayImpactEffect(HitResult);
		UE_LOG(LogTemp, Warning, TEXT("MeleeCollide is occured, RequestServer to DEAL DAMAGE!"));
	}
}

void ASCharacterBase_Kallari::OnRoundCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsLocallyControlled() && OtherActor != this)
	{
		const USkill* TargetSkill = AbilityComp->GetCurrentSkill();
		float RealDamage = TargetSkill ? TargetSkill->GetDamage() : NormalDamage;

		ServerRequestDealDamage(OtherActor, RealDamage);

		FHitResult HitResult(OtherActor, OtherComp, OtherActor->GetActorLocation(), (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal());
		ServerPlayImpactEffect(HitResult);
		UE_LOG(LogTemp, Warning, TEXT("RoundCollide is occured, RequestServer to DEAL DAMAGE!"));
	}
}