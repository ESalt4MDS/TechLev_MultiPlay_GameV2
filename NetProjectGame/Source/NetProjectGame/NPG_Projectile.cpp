// Fill out your copyright notice in the Description page of Project Settings.


#include "NPG_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NetProjectGameCharacter.h"
#include "NPG_PlayerState.h"

// Sets default values
ANPG_Projectile::ANPG_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");

	SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
	SphereCollision->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);
	RootComponent = SphereCollision;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Projectile Mesh");
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->SetupAttachment(RootComponent);


	bReplicates = true;
}

// Called when the game starts or when spawned
void ANPG_Projectile::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->IgnoreActorWhenMoving(GetOwner(), true);
	if (ANetProjectGameCharacter* OwnerCharacter = Cast<ANetProjectGameCharacter>(GetOwner()))
	{
		OwnerCharacter->MoveIgnoreActorAdd(this);

		OwnerPlayerState = OwnerCharacter->GetPlayerState<ANPG_PlayerState>();
		
	}
	
}

void ANPG_Projectile::OnProjectileHit(UPrimitiveComponent* _ThisHitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& _HitResult)
{
	if (HasAuthority())
	{
		if (ANetProjectGameCharacter* HitCharacter = Cast<ANetProjectGameCharacter>(_OtherActor))
		{
			HitCharacter->NPG_TakeDamage(Damage, OwnerPlayerState);
		}

		Destroy();
	}
}

// Called every frame
void ANPG_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

