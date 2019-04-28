// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "SProjectileRadial.generated.h"

/**
 * 
 */
UCLASS()
class SPROJECT_API ASProjectileRadial : public ASProjectile
{
	GENERATED_BODY()
	
public:
	ASProjectileRadial();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;

public:
	UPROPERTY(EditDefaultsOnly)
	float ExplodeRadius;
};
