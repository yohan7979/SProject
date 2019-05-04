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
#include "GameFramework/CharacterMovementComponent.h"
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
	BoostImpulseIntensity = 2000.f;
	BoostDirectionDegree = 30.f;
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
	bool bCasting = false;
	if (AbilityComp)
	{
		USkill_Toggle* ToggleSkill = Cast<USkill_Toggle>(AbilityComp->GetTargetSkill(ESkillType::Two));
		if (ToggleSkill)
		{
			bCasting = ToggleSkill->IsCasting();
		}
	}

	// 2번 스킬 캐스팅 중 공격 명령 시, End모션 실행
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

void ASCharacterBase_Kallari::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCharacterBase_Kallari::NotifiedSkillFinished(ESkillType SkillType)
{
	Super::NotifiedSkillFinished(SkillType);

	bool bCasting = false;

	switch (SkillType)
	{
	case ESkillType::Two:
		if (AbilityComp)
		{
			USkill_Toggle* ToggleSkill = Cast<USkill_Toggle>(AbilityComp->GetTargetSkill(SkillType));
			if (ToggleSkill)
			{
				bCasting = ToggleSkill->IsCasting();
			}
		}

		// 2번 스킬 캐스팅 중 종료 시, 캔슬 재생
		if (bCasting)
		{
			ExecuteAbility(EAnimMontageType::CancelMotion, SkillType);
		}
		break;
	default:
		break;
	}
}

void ASCharacterBase_Kallari::NotifyAnimationPlayed(FName AnimName)
{
	if (AnimName == "Boost_Start")
	{
		Super::DoAttack();
	}
}

void ASCharacterBase_Kallari::Landed(const FHitResult& Hit)
{	
	if (AnimationHandler->IsMontangePlaying(EAnimMontageType::Ability_Three))
	{
		DoSpeicalAction(EAnimMontageType::Ability_Three, ESkillType::Three, "End");
		if (Role < ROLE_Authority)
		{
			ServerDoSpecialAction(EAnimMontageType::Ability_Three, ESkillType::Three, "End");
		}
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->FallingLateralFriction = 0.f;
		GetCharacterMovement()->GravityScale = 1.f;
	}
}

void ASCharacterBase_Kallari::ExecuteAbilityOne(EAnimMontageType & eAnimType, FName SectionName)
{
}

void ASCharacterBase_Kallari::ExecuteAbilityTwo(EAnimMontageType & eAnimType, FName SectionName)
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
				ReadyToDaggerThrow();
			}
			// 2차 시전 : 던지기 OR 취소
			else
			{
				// 던지기의 경우 셋타이머
				if (SectionName == "End")
				{
					GetWorldTimerManager().SetTimer(TimerHandle_DaggerThrow, this, &ASCharacterBase_Kallari::ServerCreateDagger, 0.2f, false);
				}
				// 취소
				else
				{
					eAnimType = EAnimMontageType::CancelMotion;
				}
			
				EndThrowDagger();
			}
		}
	}
}

void ASCharacterBase_Kallari::ExecuteAbilityThree(EAnimMontageType & eAnimType, FName SectionName)
{
	ServerBoostSimluation();
}

void ASCharacterBase_Kallari::ExecuteAbilityFour(EAnimMontageType & eAnimType, FName SectionName)
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

void ASCharacterBase_Kallari::ServerBoostSimluation_Implementation()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();

	if (MovementComp)
	{
		MovementComp->Velocity = FVector::ZeroVector;
		MovementComp->FallingLateralFriction = 0.5f;
		MovementComp->GravityScale = 1.5f;
		MovementComp->AddImpulse(GetActorForwardVector().RotateAngleAxis(-BoostDirectionDegree, GetActorRightVector()) * BoostImpulseIntensity, true);
	}
}

bool ASCharacterBase_Kallari::ServerBoostSimluation_Validate()
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
		Projectile->SetProjectileDamage(GetSkillDamage());
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
