// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "SAnimationHandler.h"
#include "SAttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

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

	// 콤보 카운팅 초기화
	if (ComboCount > 0 && GetWorld()->TimeSeconds - LastAttackTime > ComboCountKeepingTime)
	{
		ComboCount = 0;
	}
}

void ASCharacterBase::Move(const FVector& Direction, float fValue)
{
	AddMovementInput(Direction, fValue);
}

void ASCharacterBase::BeginAttack()
{
	if (GetWorldTimerManager().IsTimerActive(Timer_AttackCooldown))
		return;

	float FirstDelay = FMath::Max(0.f,  LastAttackTime - GetWorld()->GetTimeSeconds() + AttackCooldown);
	GetWorldTimerManager().SetTimer(Timer_AttackCooldown, this, &ASCharacterBase::DoAttack, AttackCooldown, true, FirstDelay);
}

void ASCharacterBase::EndAttack()
{
	GetWorldTimerManager().ClearTimer(Timer_AttackCooldown);
}

void ASCharacterBase::DoAttack()
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

void ASCharacterBase::OnHealthChanged(float CurrentHealth, float DamageAmount, AActor* DamageCauser, AController* InstigatorController)
{
	UE_LOG(LogTemp, Warning, TEXT("Causer : %s, Actor: %s, CurrentHealth : %f, Damage : %f"), *DamageCauser->GetName(), *GetName(), CurrentHealth, DamageAmount);
}

void ASCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASCharacterBase::DoSpeicalAction(EAnimMontageType eType)
{
	if (AnimationHandler != nullptr)
	{
		AnimationHandler->PlayAnimationMontage(eType);
	}
}

void ASCharacterBase::MulticastDoSpecialAction_Implementation(EAnimMontageType eType)
{
	// SimulatedProxy를 대상으로만 수행한다. (데디는 실행할 필요없고, AutonomousProxy는 로컬에서 실행하였음)
	if (Role == ROLE_SimulatedProxy)
	{
		DoSpeicalAction(eType);
	}
}

bool ASCharacterBase::MulticastDoSpecialAction_Validate(EAnimMontageType eType)
{
	return true;
}

void ASCharacterBase::ServerDoSpecialAction_Implementation(EAnimMontageType eType)
{
	// 서버에서 액션을 멀티캐스트한다.
	MulticastDoSpecialAction(eType);
}

bool ASCharacterBase::ServerDoSpecialAction_Validate(EAnimMontageType eType)
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
