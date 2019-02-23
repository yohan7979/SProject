// Fill out your copyright notice in the Description page of Project Settings.

#include "SAnimationHandler.h"
#include "Animation/AnimMontage.h"

USAnimationHandler::USAnimationHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USAnimationHandler::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacter = Cast<ASCharacterBase>(GetOwner());
}


// Called every frame
void USAnimationHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USAnimationHandler::PlayAnimationMontage(EAnimMontageType eAnimType)
{
	if (CachedCharacter == nullptr)
		return;

	if (AnimMontageList[eAnimType] != nullptr)
	{
		CachedCharacter->PlayAnimMontage(AnimMontageList[eAnimType]);
	}
}

