// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangedSigniture, float, CurrentHealth, float, DamageAmount, AActor*, DamageCauser, AController*, InstigatorController);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPROJECT_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USAttributeComponent();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() { return CurrentHealth; }
	void SetCurrentHealth(float InHealth) { CurrentHealth = InHealth; }
	void AddCurrentHealth(float InHealth) { FMath::Max(0.f, CurrentHealth += InHealth); }

	UFUNCTION(BlueprintCallable)
	float GetCurrentMana() { return CurrentMana; }
	void SetCurrentMana(float InMana) { CurrentMana = InMana; }
	void AddCurrentMana(float InMana) { FMath::Max(0.f, CurrentMana += InMana); }

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	UPROPERTY(Replicated)
	float CurrentHealth;
	float MaxHelth;

	UPROPERTY(Replicated)
	float CurrentMana;
	float MaxMana;

public:
	UPROPERTY(BlueprintAssignable, Category="Event")
	FOnHealthChangedSigniture OnHealthChanged;


};
