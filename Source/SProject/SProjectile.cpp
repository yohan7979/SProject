
#include "SProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "SCharacterBase.h"

ASProjectile::ASProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(25.f);
	SphereComp->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = SphereComp;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->SetUpdatedComponent(SphereComp);
	ProjectileMovementComp->bShouldBounce = false;
	ProjectileMovementComp->InitialSpeed = 3000.f;
	ProjectileMovementComp->MaxSpeed = 3000.f;

	EffectPSComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectPSComp"));
	EffectPSComp->SetupAttachment(SphereComp);

	InitialLifeSpan = 3.f;
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ASProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 충돌처리는 Server, Client 모두 수행한다. (Simulation을 위해)
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectile::OnHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASProjectile::OnOverlapped);
	PlayEffect(SpawnPS);
}

// Called every frame
void ASProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASProjectile::Destroyed()
{
	PlayEffect(HitNonePS);
	
	Super::Destroyed();
}

void ASProjectile::SetProjectileDirection(const FVector& Direction)
{
	ProjectileMovementComp->Velocity = Direction * ProjectileMovementComp->InitialSpeed;
}

void ASProjectile::PlayEffect(TSoftObjectPtr<UParticleSystem> TargetPS)
{
	if (GetNetMode() == NM_DedicatedServer)
		return;

	if (TargetPS.IsValid())
	{
		UParticleSystem* LoadedPS = TargetPS.LoadSynchronous();
		if (LoadedPS)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LoadedPS, GetTransform());
		}
	}
}

void ASProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	PlayEffect(HitWorldPS);
	Destroy();
}

void ASProjectile::OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	ASCharacterBase* HitActor = Cast<ASCharacterBase>(OtherActor);
	ASCharacterBase* OwnerPawn = Cast<ASCharacterBase>(GetOwner());
	
	if (HitActor && OwnerPawn && HitActor != OwnerPawn)
	{
		if (Role == ROLE_Authority)
		{
			// 서버에서 Overlapped 판정일 때 데미지를 준다.
			OwnerPawn->ServerRequestDealDamage(HitActor, OwnerPawn->GetSkillDamage());
		}
		
		PlayEffect(HitPawnPS);
		Destroy();
	}
}

void ASProjectile::AddPreloadContent(FPreloadContentContainer& Collector, bool bIsDedicateServer)
{
	if (!bIsDedicateServer)
	{
		Collector.Add(SpawnPS);
		Collector.Add(HitWorldPS);
		Collector.Add(HitPawnPS);
		Collector.Add(HitNonePS);
	}
}
