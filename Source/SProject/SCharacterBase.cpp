// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "SAnimationHandler.h"
#include "SAbilityComponent.h"
#include "SAttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "SPlayerController.h"
#include "Skill.h"
#include "SProjectile.h"

ASCharacterBase::ASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetRelativeLocation(FVector(0.f, 60.f, 100.f));
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	AnimationHandler = CreateDefaultSubobject<USAnimationHandler>(TEXT("AnimationHandler"));
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));
	AbilityComp = CreateDefaultSubobject<USAbilityComponent>(TEXT("AbilityComp"));

	ComboCount = 0;
}

void ASCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacterBase::OnHealthChanged);
}

void ASCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCharacterBase::Move(const FVector& Direction, float fValue)
{
	AddMovementInput(Direction, fValue);
}

void ASCharacterBase::BeginAttack()
{
	if (GetWorldTimerManager().IsTimerActive(TimerHandle_AttackCooldown))
		return;

	float FirstDelay = FMath::Max(0.f,  LastAttackTime + AttackCooldown - GetWorld()->GetTimeSeconds());
	GetWorldTimerManager().SetTimer(TimerHandle_AttackCooldown, this, &ASCharacterBase::DoAttack, AttackCooldown, true, FirstDelay);
}

void ASCharacterBase::EndAttack()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_AttackCooldown);
}

void ASCharacterBase::BeginSubAttack()
{

}

void ASCharacterBase::EndSubAttack()
{

}

void ASCharacterBase::DoAttack()
{
	EAnimMontageType	eAnimType;
	ESkillType			eSkillType;

	GetAnimMontageByComboCount(eAnimType, eSkillType);

	if (ExecuteAbility(eAnimType, eSkillType))
	{
		CalculateComboCount();
		LastAttackTime = GetWorld()->GetTimeSeconds();
	}
}

void ASCharacterBase::DoJump()
{
	Jump();
}

void ASCharacterBase::StopJump()
{
	StopJumping();
}

bool ASCharacterBase::ExecuteAbility(EAnimMontageType eAnimType, ESkillType eSkillType, FName SectionName)
{
	// 발동할 수 있는 지 검사
	if (AbilityComp && AbilityComp->CanExecute(eSkillType))
	{
		// 스킬 발동 (코스트 처리)
		AbilityComp->ExecuteSkill(eSkillType);

		// 각 스킬별 애님몽타주 설정
		switch (eSkillType)
		{
		case ESkillType::One:
			ExecuteAbilityOne(eAnimType, SectionName);
			break;
		case ESkillType::Two:
			ExecuteAbilityTwo(eAnimType, SectionName);
			break;
		case ESkillType::Three:
			ExecuteAbilityThree(eAnimType, SectionName);
			break;
		case ESkillType::Four:
			ExecuteAbilityFour(eAnimType, SectionName);
			break;
		default:
			break;
		}

		// 애니메이션 처리
		DoSpeicalAction(eAnimType, eSkillType, SectionName);
		if (Role < ROLE_Authority)
		{
			ServerDoSpecialAction(eAnimType, eSkillType, SectionName);
		}
		return true;
	}

	return false;
}

void ASCharacterBase::ExecuteAbilityOne(EAnimMontageType & eAnimType, FName SectionName)
{
}

void ASCharacterBase::ExecuteAbilityTwo(EAnimMontageType & eAnimType, FName SectionName)
{
}

void ASCharacterBase::ExecuteAbilityThree(EAnimMontageType & eAnimType, FName SectionName)
{
}

void ASCharacterBase::ExecuteAbilityFour(EAnimMontageType & eAnimType, FName SectionName)
{
}

void ASCharacterBase::PlayImpactEffect(const FHitResult& HitResult)
{
	const USkill* TargetSkill = AbilityComp->GetCurrentSkill();
	AActor* HitActor = HitResult.GetActor();
	if (TargetSkill && HitActor)
	{
		UParticleSystem* ParticleToPlay = nullptr;
		if (Cast<ASCharacterBase>(HitActor) != nullptr)
		{
			ParticleToPlay = TargetSkill->GetParticleSystemPawn();
		}
		else
		{
			ParticleToPlay = TargetSkill->GetParticleSystemWorld();
		}

		if (ParticleToPlay != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleToPlay, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
		}
	}
}

void ASCharacterBase::SetWeaponCollision(EWeaponCollisionType eType, bool bEnable)
{
	UPrimitiveComponent* TargetCollComp = nullptr;

	switch (eType)
	{
	case EWeaponCollisionType::Melee:
		TargetCollComp = MeleeCollComp;
		break;
	case EWeaponCollisionType::Round:
		TargetCollComp = RoundCollComp;
		break;
	default:;
		return;
	}

	if (TargetCollComp != nullptr)
	{
		TargetCollComp->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void ASCharacterBase::GetAnimMontageByComboCount(EAnimMontageType& eAnimType, ESkillType& eSkillType)
{
	// PC에서 처리한 입력을 받아 액션 수행하고, 서버에게 호출한다.
	EAnimMontageType DesiredAnim;
	ESkillType DesiredSkill;

	if (GetCharacterMovement()->IsFalling() && GetVelocity().Z >= 0.f)
	{
		DesiredAnim = EAnimMontageType::JumpAttack;
		DesiredSkill = ESkillType::Basic_A;
	}
	else
	{
		switch (ComboCount)
		{
		case 0:
			DesiredAnim = EAnimMontageType::NormalAttack_A;
			DesiredSkill = ESkillType::Basic_A;
			break;
		case 1:
			DesiredAnim = EAnimMontageType::NormalAttack_B;
			DesiredSkill = ESkillType::Basic_B;
			break;
		case 2:
			DesiredAnim = EAnimMontageType::NormalAttack_C;
			DesiredSkill = ESkillType::Basic_C;
			break;
		case 3:
			DesiredAnim = EAnimMontageType::NormalAttack_D;
			DesiredSkill = ESkillType::Basic_D;
			break;
		default:
			DesiredAnim = EAnimMontageType::NormalAttack_A;
			DesiredSkill = ESkillType::Basic_A;
			break;
		}
	}

	eAnimType = DesiredAnim;
	eSkillType = DesiredSkill;
}

void ASCharacterBase::CalculateComboCount()
{
	// 로컬 클라이언트에서 쿨타임, 콤보 카운팅 계산
	if (bRandomCombo)
	{
		ComboCount = FMath::Rand() % MaxComboCount;
	}
	else if(GetCharacterMovement()->IsFalling())
	{
		ResetComboCount();
	}
	else
	{
		ComboCount < MaxComboCount - 1 ? ++ComboCount : ComboCount = 0;
	}

	GetWorldTimerManager().ClearTimer(TimerHandle_Combo);
	GetWorldTimerManager().SetTimer(TimerHandle_Combo, this, &ASCharacterBase::ResetComboCount, ComboCountKeepingTime, false);
}

void ASCharacterBase::ResetComboCount()
{
	ComboCount = 0;
}

void ASCharacterBase::NotifiedSkillFinished(ESkillType SkillType)
{
	UE_LOG(LogTemp, Log, TEXT("Skill %d is Finished."), static_cast<int>(SkillType));
}

float ASCharacterBase::GetSkillDamage() const
{
	if (AbilityComp != nullptr)
	{
		USkill* TargetSkill = AbilityComp->GetCurrentSkill();
		if (TargetSkill != nullptr)
		{
			return TargetSkill->GetDamage();
		}
	}

	return NormalDamage;
}

void ASCharacterBase::NotifyAnimationPlayed(FName AnimName)
{

}

void ASCharacterBase::OnHealthChanged(float CurrentHealth, float DamageAmount, AActor* DamageCauser, AController* InstigatorController)
{
	if (CurrentHealth <= 0.f && !bDied)
	{
		ServerSetDied(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("Causer : %s, Actor: %s, CurrentHealth : %f, Damage : %f"), *DamageCauser->GetName(), *GetName(), CurrentHealth, DamageAmount);
}

void ASCharacterBase::DoSpeicalAction(EAnimMontageType eAnimType, ESkillType eSkillType, FName SectionName)
{
	if (AnimationHandler != nullptr)
	{
		AnimationHandler->PlayAnimationMontage(eAnimType, SectionName);
	}

	if (AbilityComp != nullptr)
	{
		AbilityComp->SetCurrentSkillType(eSkillType);
	}
}

void ASCharacterBase::OnRep_Died()
{
	if (bDied)
	{
		int iDeathIndex = FMath::Rand() % 2;
		DoSpeicalAction(iDeathIndex == 0 ? EAnimMontageType::Death_A : EAnimMontageType::Death_B);

		// 클라이언트 자신에 해당
		if (IsLocallyControlled())
		{
			ASPlayerController* PC = Cast<ASPlayerController>(GetController());
			if (PC)
			{
				PC->SetControlledPawn(nullptr);
				PC->UnPossess();
			}
		}

		// 콜리젼 off
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (MeleeCollComp)			MeleeCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (RoundCollComp)			RoundCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASCharacterBase::ServerSetDied_Implementation(bool isDie)
{
	bDied = isDie;

	OnRep_Died();
}

bool ASCharacterBase::ServerSetDied_Validate(bool isDie)
{
	return true;
}

void ASCharacterBase::MulticastDoSpecialAction_Implementation(EAnimMontageType eAnimType, ESkillType eSkillType, FName SectionName)
{
	if (Role == ROLE_AutonomousProxy)
	{
		return;
	}

	DoSpeicalAction(eAnimType, eSkillType, SectionName);
}

void ASCharacterBase::ServerDoSpecialAction_Implementation(EAnimMontageType eAnimType, ESkillType eSkillType, FName SectionName)
{
	// 서버에서 액션을 멀티캐스트한다.
	MulticastDoSpecialAction(eAnimType, eSkillType, SectionName);
}

bool ASCharacterBase::ServerDoSpecialAction_Validate(EAnimMontageType eAnimType, ESkillType eSkillType, FName SectionName)
{
	return true;
}

void ASCharacterBase::ServerRequestDealDamage_Implementation(AActor* OtherActor, float BaseDamage)
{
	UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, GetController(), this, nullptr);
	UE_LOG(LogTemp, Warning, TEXT("Deal DAMAGE, Amount : %f"), BaseDamage);
}

bool ASCharacterBase::ServerRequestDealDamage_Validate(AActor* OtherActor, float BaseDamage)
{
	return BaseDamage != 0.f;
}

void ASCharacterBase::MulticastPlayImpactEffect_Implementation(const FHitResult& HitResult)
{
	// 데디 서버는 이펙트 출력할 필요없다.
	if (GetNetMode() == NM_DedicatedServer)
		return;

	PlayImpactEffect(HitResult);
}

void ASCharacterBase::ServerPlayImpactEffect_Implementation(const FHitResult& HitResult)
{
	MulticastPlayImpactEffect(HitResult);
}

bool ASCharacterBase::ServerPlayImpactEffect_Validate(const FHitResult& HitResult)
{
	return true;
}

bool ASCharacterBase::DoHitScanTrace(FHitResult& HitResult, const FVector& StartLocation /*= FVector::ZeroVector*/) const
{
	FVector StartTrace = StartLocation.IsNearlyZero() ? GetPawnViewLocation() : StartLocation;
	FVector AimDirection = GetControlRotation().Vector();
	FVector EndTrace = StartTrace + AimDirection * 10000.f; // TODO: Make Range

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	return GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_GameTraceChannel1, CollisionParams);
}

void ASCharacterBase::CreateProjectile(UClass* InClass, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	ASProjectile* Projectile = GetWorld()->SpawnActor<ASProjectile>(InClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (Projectile != nullptr)
	{
		Projectile->SetProjectileDirection(SpawnRotation.Vector().GetSafeNormal());
		Projectile->SetProjectileDamage(GetSkillDamage());
	}
}

void ASCharacterBase::AddPreloadContent(FPreloadContentContainer& Collector, bool bIsDedicateServer)
{
	
}

void ASCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacterBase, bDied);
}