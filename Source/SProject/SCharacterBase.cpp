// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "SAnimationHandler.h"
#include "SAbilityComponent.h"
#include "SAttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "SPlayerController.h"
#include "Skill.h"

ASCharacterBase::ASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
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

void ASCharacterBase::DoAttack()
{
	// PC에서 처리한 입력을 받아 액션 수행하고, 서버에게 호출한다.
	EAnimMontageType DesiredType = EAnimMontageType::EAMT_NormalAttack_A;
	switch (ComboCount)
	{
	case 0:
		DesiredType = EAnimMontageType::EAMT_NormalAttack_A;
		break;
	case 1:
		DesiredType = EAnimMontageType::EAMT_NormalAttack_B;
		break;
	case 2:
		DesiredType = EAnimMontageType::EAMT_NormalAttack_C;
		break;
	}

	LastAttackTime = GetWorld()->GetTimeSeconds();

	DoSpeicalAction(DesiredType);

	if (Role < ROLE_Authority)
	{
		ServerDoSpecialAction(DesiredType);
	}
}

void ASCharacterBase::DoJump()
{
	Jump();

	ResetComboCount();
}

void ASCharacterBase::StopJump()
{
	StopJumping();
}

bool ASCharacterBase::ExecuteAbilityOne()
{
	if (AbilityComp != nullptr)
	{
		return AbilityComp->ExecuteSkill(ESkillType::EAST_One);
	}

	return false;
}

bool ASCharacterBase::ExecuteAbilityTwo()
{
	if (AbilityComp != nullptr)
	{
		return AbilityComp->ExecuteSkill(ESkillType::EAST_Two);
	}

	return false;
}

bool ASCharacterBase::ExecuteAbilityThree()
{
	if (AbilityComp != nullptr)
	{
		return AbilityComp->ExecuteSkill(ESkillType::EAST_Three);
	}

	return false;
}

bool ASCharacterBase::ExecuteAbilityFour()
{
	if (AbilityComp != nullptr)
	{
		return AbilityComp->ExecuteSkill(ESkillType::EAST_Four);
	}

	return false;
}

void ASCharacterBase::PlayImpactEffect(const FHitResult& HitResult)
{

}

void ASCharacterBase::SetWeaponCollision(EWeaponCollisionType eType, bool bEnable)
{
	UCapsuleComponent* TargetCollComp = nullptr;

	switch (eType)
	{
	case EWeaponCollisionType::EWCT_Left:
		TargetCollComp = LeftWeaponCollComp;
		break;
	case EWeaponCollisionType::EWCT_Right:
		TargetCollComp = RightWeaponCollComp;
		break;
	default:;
		return;
	}

	if (TargetCollComp != nullptr)
	{
		TargetCollComp->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void ASCharacterBase::ResetComboCount()
{
	ComboCount = 0;
}

void ASCharacterBase::OnHealthChanged(float CurrentHealth, float DamageAmount, AActor* DamageCauser, AController* InstigatorController)
{
	if (CurrentHealth <= 0.f && !bDied)
	{
		ServerSetDied(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("Causer : %s, Actor: %s, CurrentHealth : %f, Damage : %f"), *DamageCauser->GetName(), *GetName(), CurrentHealth, DamageAmount);
}

void ASCharacterBase::DoSpeicalAction(EAnimMontageType eAnimType, ESkillType eSkillType)
{
	if (AnimationHandler != nullptr)
	{
		AnimationHandler->PlayAnimationMontage(eAnimType);
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
		DoSpeicalAction(iDeathIndex == 0 ? EAnimMontageType::EAMT_Death_A : EAnimMontageType::EAMT_Death_B);

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
		if (LeftWeaponCollComp)		LeftWeaponCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (RightWeaponCollComp)	RightWeaponCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void ASCharacterBase::MulticastDoSpecialAction_Implementation(EAnimMontageType eAnimType, ESkillType eSkillType)
{
	// SimulatedProxy를 대상으로만 수행한다. (데디는 실행할 필요없고, AutonomousProxy는 로컬에서 실행하였음)
	if (Role == ROLE_SimulatedProxy)
	{
		DoSpeicalAction(eAnimType, eSkillType);
	}
}

void ASCharacterBase::ServerDoSpecialAction_Implementation(EAnimMontageType eAnimType, ESkillType eSkillType)
{
	// 서버에서 액션을 멀티캐스트한다.
	MulticastDoSpecialAction(eAnimType, eSkillType);
}

bool ASCharacterBase::ServerDoSpecialAction_Validate(EAnimMontageType eAnimType, ESkillType eSkillType)
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

void ASCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacterBase, bDied);
}