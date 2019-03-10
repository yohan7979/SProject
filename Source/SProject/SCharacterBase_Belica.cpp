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
	MaxComboCount = 1;
	MuzzleSocketName = "Muzzle_01";
}

void ASCharacterBase_Belica::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacterBase_Belica::DoAttack()
{
	Super::DoAttack();

	//클라이언트에서 LineTrace를 하고, Hit이 성공하면 서버에게 데미지 요청 + PlayImpact
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

	/** Muzzle 이펙트는 애님노티로 출력한다. */
}