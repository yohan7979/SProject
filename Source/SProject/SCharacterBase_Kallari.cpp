// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacterBase_Kallari.h"
#include "SAttributeComponent.h"
#include "SAbilityComponent.h"
#include "Skill.h"
#include "Skill_Toggle.h"
#include "SAnimationHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "SProjectile.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"

ASCharacterBase_Kallari::ASCharacterBase_Kallari()
{
	PrimaryActorTick.bCanEverTick = true;

	MeleeCollComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCollComp"));
	MeleeCollComp->SetupAttachment(RootComponent);

	RoundCollComp = CreateDefaultSubobject<USphereComponent>(TEXT("RoundCollComp"));
	RoundCollComp->SetupAttachment(RootComponent);

	// Timeline Delegate <float>
	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));
	OnTimelineFloat_Dagger.BindUFunction(this, FName("TimelineFloatReturn"));
	OnTimelineEvent_Dagger.BindUFunction(this, FName("TimelineFinished"));

	AttackCooldown = 1.f;
	ComboCountKeepingTime = 1.5f;
	NormalDamage = 20.f;
	MaxComboCount = 3;
	DaggerRange = 2000.f;
	DaggerSocketName = "dagger_a_r";
}

void ASCharacterBase_Kallari::BeginPlay()
{
	Super::BeginPlay();

	if (MeleeCollComp != nullptr)
		MeleeCollComp->OnComponentBeginOverlap.AddDynamic(this, &ASCharacterBase_Kallari::OnMeleeCollisionBeginOverlap);

	if (RoundCollComp != nullptr)
		RoundCollComp->OnComponentBeginOverlap.AddDynamic(this, &ASCharacterBase_Kallari::OnRoundCollisionBeginOverlap);

	if (TimelineComp && CurveFloat)
	{
		TimelineComp->AddInterpFloat(CurveFloat, OnTimelineFloat_Dagger);
		TimelineComp->SetTimelineFinishedFunc(OnTimelineEvent_Dagger);
		TimelineComp->SetLooping(false);

		StartFOV = CameraComp->FieldOfView;
	}
}

void ASCharacterBase_Kallari::DoAttack()
{
	if (AbilityComp && AbilityComp->IsSkillActivated(ESkillType::EAST_Two))
	{
		AbilityComp->ExecuteSkill(ESkillType::EAST_Two);

		// 애니메이션
		DoSpeicalAction(EAnimMontageType::EAMT_Ability_Two, ESkillType::EAST_Two, "End");
		if (Role < ROLE_Authority)
		{
			ServerDoSpecialAction(EAnimMontageType::EAMT_Ability_Two, ESkillType::EAST_Two, "End");
		}

		EndThrowDagger();
		GetWorldTimerManager().SetTimer(TimerHandle_DaggerThrow, this, &ASCharacterBase_Kallari::ServerCreateDagger, 0.2f);
	}
	else
	{
		Super::DoAttack();
	}
}

void ASCharacterBase_Kallari::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCharacterBase_Kallari::NotifiedSkillFinished(ESkillType SkillType)
{
	Super::NotifiedSkillFinished(SkillType);

	switch (SkillType)
	{
	case ESkillType::EAST_Two:
		AbilityComp->ExecuteSkill(SkillType);

		DoSpeicalAction(EAnimMontageType::EAMT_CancelMotion);
		if (Role < ROLE_Authority)
		{
			ServerDoSpecialAction(EAnimMontageType::EAMT_CancelMotion);
		}

		EndThrowDagger();	
		break;
	default:
		break;
	}
}

void ASCharacterBase_Kallari::ExecuteAbilityOne(EAnimMontageType & eAnimType)
{
}

void ASCharacterBase_Kallari::ExecuteAbilityTwo(EAnimMontageType & eAnimType)
{
	bool bCasting = false;
	if (AbilityComp)
	{
		USkill_Toggle* ToggleSkill = Cast<USkill_Toggle>(AbilityComp->GetTargetSkill(ESkillType::EAST_Two));
		if (ToggleSkill)
		{
			bCasting = ToggleSkill->IsCasting();
		}
	}

	// 1차 시전 : 던지기 준비
	if (bCasting)
	{
		ReadyToDaggerThrow();
	}
	// 2차 시전 : 던지기 취소
	else
	{
		eAnimType = EAnimMontageType::EAMT_CancelMotion;
		EndThrowDagger();
	}
}

void ASCharacterBase_Kallari::ExecuteAbilityThree(EAnimMontageType & eAnimType)
{
}

void ASCharacterBase_Kallari::ExecuteAbilityFour(EAnimMontageType & eAnimType)
{
}

void ASCharacterBase_Kallari::ServerCreateDagger_Implementation()
{
	ThrowDagger();
}

bool ASCharacterBase_Kallari::ServerCreateDagger_Validate()
{
	return true;
}

void ASCharacterBase_Kallari::ThrowDagger()
{
	if (Role != ROLE_Authority)
		return;

	// 프로젝타일 - Spawn On Server
	FVector StartTrace = GetPawnViewLocation();
	FVector AimDir = GetControlRotation().Vector();
	FVector EndTrace = StartTrace + AimDir * 10000.f;

	FHitResult Hit(1.f);
	FCollisionQueryParams CollisionQueries;
	CollisionQueries.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_GameTraceChannel1, CollisionQueries))
	{
		AimDir = Hit.ImpactPoint - StartTrace;
		AimDir.Normalize();
	}

	FVector SocketLocation = GetMesh()->GetSocketLocation(DaggerSocketName);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	
	ASProjectile* Projectile = GetWorld()->SpawnActor<ASProjectile>(DaggerProjectileClass, SocketLocation, AimDir.Rotation(), SpawnParams);
	if (Projectile != nullptr)
	{
		Projectile->SetProjectileDirection(AimDir);
	}

	//DrawDebugLine(GetWorld(), SocketLocation, SocketLocation + AimDir * DaggerRange, FColor::Red, false, 3.f);
}

void ASCharacterBase_Kallari::ReadyToDaggerThrow()
{
	// For Client
	if (TimelineComp != nullptr)
	{
		TimelineComp->Play();
	}
}

void ASCharacterBase_Kallari::EndThrowDagger()
{
	if (TimelineComp != nullptr)
	{
		TimelineComp->Reverse();
	}
}

void ASCharacterBase_Kallari::OnMeleeCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsLocallyControlled() && OtherActor != this)
	{
		ServerRequestDealDamage(OtherActor, GetSkillDamage());

		FHitResult HitResult(OtherActor, OtherComp, OtherActor->GetActorLocation(), (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal());
		ServerPlayImpactEffect(HitResult);
		UE_LOG(LogTemp, Warning, TEXT("MeleeCollide is occured, RequestServer to DEAL DAMAGE!"));
	}
}

void ASCharacterBase_Kallari::OnRoundCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsLocallyControlled() && OtherActor != this)
	{
		ServerRequestDealDamage(OtherActor, GetSkillDamage());

		FHitResult HitResult(OtherActor, OtherComp, OtherActor->GetActorLocation(), (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal());
		ServerPlayImpactEffect(HitResult);
		UE_LOG(LogTemp, Warning, TEXT("RoundCollide is occured, RequestServer to DEAL DAMAGE!"));
	}
}

void ASCharacterBase_Kallari::TimelineFloatReturn(float value)
{
	CameraComp->SetFieldOfView(FMath::Lerp<float>(StartFOV, EndFOV, value));
}

void ASCharacterBase_Kallari::TimelineFinished()
{

}
