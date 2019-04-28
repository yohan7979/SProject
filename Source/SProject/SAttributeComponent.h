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
	void AddCurrentMana(float InMana);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetCurrentMana(float InMana);

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void HandleTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

private:
	UPROPERTY(Replicated)
	float CurrentHealth;
	UPROPERTY(Replicated)
	float CurrentMana;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float MaxHelth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float MaxMana;

	UPROPERTY(BlueprintAssignable, Category="Event")
	FOnHealthChangedSigniture OnHealthChanged;
};
