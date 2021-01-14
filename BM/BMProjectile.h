// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BMProjectile.generated.h"


UCLASS()
class BOSSMODE_API ABMProjectile : public AActor
{
	GENERATED_BODY()

	/** 구체 콜리전 컴포넌트 */
	UPROPERTY(VisibleDefaultsOnly,Category=Projectile)
		class USphereComponent* ProjCollision;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* ProjParticle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "ture"))
		class UStaticMeshComponent* ProjMesh;

	/** 발사체 움직임 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "ture"))
		class UProjectileMovementComponent* ProjMovement;

public:	
	// Sets default values for this actor's properties
	ABMProjectile();

	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const
	{
		return ProjMovement;
	}

	/** 발사체가 무언가와 부딪혔을 때 호출함 */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnDestroyedProjectile(AActor* DestroyingActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
