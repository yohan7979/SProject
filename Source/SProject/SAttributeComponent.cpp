// Fill out your copyright notice in the Description page of Project Settings.

#include "SAttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "SCharacterBase.h"
#include "Math/UnrealMath.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent() : MaxHelth(200.f), MaxMana(100.f)
{
	SetIsReplicated(true);
}


void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHelth;
	CurrentMana = MaxMana;

	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			Owner->OnTakeAnyDamage.AddDynamic(this, &USAttributeComponent::HandleTakeAnyDamage);
		}
	}
}

void USAttributeComponent::ServerSetCurrentMana_Implementation(float InMana)
{
	SetCurrentMana(InMana);
}

bool USAttributeComponent::ServerSetCurrentMana_Validate(float InMana)
{
	return InMana >= 0.f;
}

void USAttributeComponent::AddCurrentMana(float InMana)
{
	float ResultMana = FMath::Max(0.f, CurrentMana += InMana);
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerSetCurrentMana(ResultMana);
	}

	SetCurrentMana(ResultMana);
}

void USAttributeComponent::HandleTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	CurrentHealth -= Damage;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHelth);

	OnHealthChanged.Broadcast(CurrentHealth, Damage, DamageCauser, InstigatedBy);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, CurrentHealth);
	DOREPLIFETIME(USAttributeComponent, CurrentMana);
}