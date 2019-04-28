// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STargetActor.generated.h"

class ASCharacterBase;
class ASPlayerController;
class USceneComponent;
class UDecalComponent;

UCLASS()
class SPROJECT_API ASTargetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTargetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ConfirmTargetingAndContinue();
	void GetPlayerLookingPoint(FVector& OutVector);

public:
	UPROPERTY(EditDefaultsOnly)
	float Radius;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;
	
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* DecalComponent;

private:
	ASCharacterBase*	OwnerPawn;
	ASPlayerController*	OwnerPC;
};
