// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
class UParticleSystem;

UCLASS()
class SPROJECT_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void SetProjectileDirection(const FVector& Direction);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayEffect(UParticleSystem* TargetPS);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EffectPSComp;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* SpawnPS;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* HitWorldPS;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* HitPawnPS;
};
