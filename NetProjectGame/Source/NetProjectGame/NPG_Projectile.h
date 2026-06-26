// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPG_Projectile.generated.h"

UCLASS()
class NETPROJECTGAME_API ANPG_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPG_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//the projectile movement component
	//This controls how fast the projectile moves, with how much gravity, whether it bounces, etc.
	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* ProjectileMovement = nullptr;

	//static mesh component
	//Controls how the projectile looks, but we won’t use it for collision.
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh = nullptr;

	//sphere collision component
	//A simple and efficient collision component, perfect for projectiles.
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* SphereCollision = nullptr;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* _ThisHitComp, AActor* _OtherActor,
		UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& _HitResult);

	UPROPERTY()
	class ANPG_PlayerState* OwnerPlayerState = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Damage = 20.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
