// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacterBase_Kallari.h"
#include "SAttributeComponent.h"
#include "SAbilityComponent.h"
#include "Skill.h"
#include "SAnimationHandler.h"
#include "Kismet/GameplayStatics.h"

ASCharacterBase_Kallari::ASCharacterBase_Kallari()
{
	LeftWeaponCollComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftWeaponCollComp"));
	LeftWeaponCollComp->SetupAttachment(GetMesh(), "WeaponAttachPointL");

	RightWeaponCollComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightWeaponCollComp"));
	RightWeaponCollComp->SetupAttachment(GetMesh(), "WeaponAttachPointR");

	AttackCooldown = 1.f;
	ComboCountKeepingTime = 1.5f;
	NormalDamage = 20.f;
	MaxComboCount = 3;
}

void ASCharacterBase_Kallari::BeginPlay()
{
	Super::BeginPlay();

	if (LeftWeaponCollComp != nullptr)
		LeftWeaponCollComp->OnComponentBeginOverlap.AddDynamic(this, &ASCharacterBase_Kallari::OnLeftCollisionBeginOverlap);

	if (RightWeaponCollComp != nullptr)
		RightWeaponCollComp->OnComponentBeginOverlap.AddDynamic(this, &ASCharacterBase_Kallari::OnRightCollisionBeginOverlap);
}

void ASCharacterBase_Kallari::DoAttack()
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

	// 로컬 클라이언트에서 쿨타임, 콤보 카운팅 계산
	if (bRandomCombo)
	{
		ComboCount = FMath::Rand() % MaxComboCount;
	}
	else
	{
		ComboCount < MaxComboCount - 1 ? ++ComboCount : ComboCount = 0;
	}

	LastAttackTime = GetWorld()->GetTimeSeconds();
	
	DoSpeicalAction(DesiredType);

	if (Role < ROLE_Authority)
	{
		ServerDoSpecialAction(DesiredType);
	}
}

bool ASCharacterBase_Kallari::ExecuteAbilityOne()
{
	// 상위 클래스에서는 AbilityComp에게 해당 Skill을 Execute하도록 명령해줌.
	// 여기서 조건 검사하고 계산 처리를 수행해준다. (코스트, 쿨다운 등)
	if (!Super::ExecuteAbilityOne())
		return false;

	// 캐릭터 구현클래스에서는 해당 캐릭터의 로직을 수행
	DoSpeicalAction(EAnimMontageType::EAMT_Ability_One);
	if (Role < ROLE_Authority)
	{
		ServerDoSpecialAction(EAnimMontageType::EAMT_Ability_One);
	}

	return true;
}

bool ASCharacterBase_Kallari::ExecuteAbilityTwo()
{
	if (!Super::ExecuteAbilityTwo())
		return false;

	return true;
}

bool ASCharacterBase_Kallari::ExecuteAbilityThree()
{
	if (!Super::ExecuteAbilityThree())
		return false;

	return true;
}

bool ASCharacterBase_Kallari::ExecuteAbilityFour()
{
	if (!Super::ExecuteAbilityFour())
		return false;

	return true;
}

void ASCharacterBase_Kallari::OnLeftCollisionBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsLocallyControlled() && OtherActor != this)
	{
		ServerRequestDealDamage(OtherActor, NormalDamage);
		UE_LOG(LogTemp, Warning, TEXT("LeftCollide is occured, RequestServer to DEAL DAMAGE!"));
	}
}

void ASCharacterBase_Kallari::OnRightCollisionBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsLocallyControlled() && OtherActor != this)
	{
		ServerRequestDealDamage(OtherActor, NormalDamage);
		UE_LOG(LogTemp, Warning, TEXT("RightCollide is occured, RequestServer to DEAL DAMAGE!"));
	}	
}