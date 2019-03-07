// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacterBase_Belica.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SceneComponent.h"

ASCharacterBase_Belica::ASCharacterBase_Belica()
{
	AttackCooldown = 0.2f;
	NormalDamage = 5.f;
	MuzzleSocketName = "Muzzle_01";
}

void ASCharacterBase_Belica::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacterBase_Belica::DoAttack()
{
	Super::DoAttack();

	//Ŭ���̾�Ʈ���� LineTrace�� �ϰ�, Hit�� �����ϸ� �������� ������ ��û + PlayImpact
	FVector StartLocation, EndLocation;
	FRotator AimDirection;
	
	StartLocation = GetPawnViewLocation();
	AimDirection = GetControlRotation();
	EndLocation = StartLocation + AimDirection.Vector() * 5000.f; // TODO: Make Range

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_GameTraceChannel1, CollisionParams))
	{
		// Request damage to server
		if (Hit.Actor != nullptr)
		{
			ServerRequestDealDamage(Hit.GetActor(), NormalDamage);

			// play impact fx (on server, multicast)
			ServerPlayImpactEffect(Hit);
		}
	}

	/** Muzzle ����Ʈ�� �ִԳ�Ƽ�� ����Ѵ�. */
}

bool ASCharacterBase_Belica::ExecuteAbilityOne()
{
	return false;
}

bool ASCharacterBase_Belica::ExecuteAbilityTwo()
{
	return false;
}

bool ASCharacterBase_Belica::ExecuteAbilityThree()
{
	return false;
}

bool ASCharacterBase_Belica::ExecuteAbilityFour()
{
	return false;
}

void ASCharacterBase_Belica::PlayImpactEffect(const FHitResult& HitResult)
{
	UParticleSystem* ParticleToPlay = nullptr;

	AActor* HitActor = HitResult.GetActor();
	if (HitActor != nullptr)
	{
		if (Cast<ASCharacterBase>(HitActor) != nullptr)
		{
			ParticleToPlay = PS_HitPawnNormal;
		}
		else
		{
			ParticleToPlay = PS_HitWorldNormal;
		}

		if (ParticleToPlay != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleToPlay, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
		}
	}
}