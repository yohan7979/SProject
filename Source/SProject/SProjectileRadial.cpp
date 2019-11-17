// Fill out your copyright notice in the Description page of Project Settings.

#include "SProjectileRadial.h"
#include "Kismet/GameplayStatics.h"
#include "SCharacterBase.h"
#include "Components/SphereComponent.h"

ASProjectileRadial::ASProjectileRadial()
{

}

void ASProjectileRadial::BeginPlay()
{
	ExplodeRadius = 200.f;
	ProjectileDamage = 500.f;
	
	Super::BeginPlay();
}

void ASProjectileRadial::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role == ROLE_Authority && !IsPendingKillPending())
	{
		ASCharacterBase* OwnerPawn = Cast<ASCharacterBase>(GetOwner());
		if (OwnerPawn)
		{
			TArray<AActor*> IgnoreActor;
			IgnoreActor.Add(this);
			IgnoreActor.Add(OwnerPawn);

			UGameplayStatics::ApplyRadialDamage(this, ProjectileDamage, GetActorLocation(), ExplodeRadius, nullptr, IgnoreActor, this, OwnerPawn->GetController(), true, ECC_GameTraceChannel1);
		}
	}

	PlayEffect(HitWorldPS);
	Destroy();
}

void ASProjectileRadial::OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

}
