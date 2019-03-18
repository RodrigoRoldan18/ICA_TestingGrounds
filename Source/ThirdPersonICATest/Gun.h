// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class THIRDPERSONICATEST_API AGun : public AActor
{
	GENERATED_BODY()

	/** Gun mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;	

	/** Location on gun mesh where projectiles should spawn */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;
	
public:	
	// Sets default values for this actor's properties
	AGun();	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;		

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Fires a projectile
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnFire();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimInstance* AnimInstance;
	
};
