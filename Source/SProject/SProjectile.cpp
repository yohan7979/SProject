
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
	if (HitNonePS != nullptr)
	{
		PlayEffect(HitNonePS);
	}

	Super::Destroyed();
}

void ASProjectile::SetProjectileDirection(const FVector& Direction)
{
	ProjectileMovementComp->Velocity = Direction * ProjectileMovementComp->InitialSpeed;
}

void ASProjectile::PlayEffect(UParticleSystem* TargetPS)
{
	if (GetNetMode() == NM_DedicatedServer)
		return;

	if (TargetPS != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TargetPS, GetTransform());
	}
}

void ASProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitWorldPS != nullptr)
	{
		PlayEffect(HitWorldPS);
	}
	
	Destroy();
}

void ASProjectile::OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	ASCharacterBase* HitActor = Cast<ASCharacterBase>(OtherActor);
	ASCharacterBase* Owner = Cast<ASCharacterBase>(GetOwner());
	
	if (HitActor && Owner && HitActor != Owner)
	{
		if (Owner->IsLocallyControlled())
		{
			// 데미지 처리 요청은, Instigator가 직접 서버에 하도록 한다.
			Owner->ServerRequestDealDamage(HitActor, Owner->GetSkillDamage());
		}
		
		if (HitPawnPS != nullptr)
		{
			PlayEffect(HitPawnPS);
		}

		Destroy();
	}
}
