// Fill out your copyright notice in the Description page of Project Settings.

#include "STargetActor.h"
#include "SCharacterBase.h"
#include "SPlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"
#include "Components/DecalComponent.h"

// Sets default values
ASTargetActor::ASTargetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(SceneComponent);

	Radius = 200.f;
	DecalComponent->DecalSize = FVector(Radius);
}

// Called when the game starts or when spawned
void ASTargetActor::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* Owner = GetOwner();
	if ( Owner )
	{
		OwnerPawn = Cast<ASCharacterBase>(Owner);

		if ( OwnerPawn )
		{
			OwnerPC = Cast<ASPlayerController>(OwnerPawn->GetController());
		}
	}

}

// Called every frame
void ASTargetActor::Tick(float DeltaTime)
{
	FVector LookingPoint;
	GetPlayerLookingPoint(LookingPoint);
	
	if (DecalComponent)
	{
		SceneComponent->SetWorldLocation(LookingPoint);
	}

	Super::Tick(DeltaTime);
}

void ASTargetActor::ConfirmTargetingAndContinue()
{
	FVector LookingPoint;

	GetPlayerLookingPoint(LookingPoint);

	
}

void ASTargetActor::GetPlayerLookingPoint(FVector& OutVector)
{
	FVector		ViewLoc;
	FRotator	ViewRot;

	if (OwnerPC)
	{
		OwnerPC->GetPlayerViewPoint(ViewLoc, ViewRot);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.AddIgnoredActor(OwnerPawn);

		FHitResult	Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, ViewLoc, ViewLoc + ViewRot.Vector() * 10000.f, ECC_Visibility, CollisionQueryParams))
		{
			OutVector = Hit.ImpactPoint;
		}
		else
		{
			OutVector = FVector::ZeroVector;
		}
	}
}

