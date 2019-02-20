// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyFirstPlayer.generated.h"

UCLASS()
class THIRDPERSONICATEST_API AMyFirstPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	//the UPROPERTY creates a variable that can be seen in the editor and edited
	//anywhere as it states in the brackets. Also the Category determines the properties
	//of the variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		int32 TotalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float DamageTimeInSeconds;

	//it's calculated using the designers settings. the visibleanywhere lets it be seen
	//but not editable. the Transient flag means that it won't be saved or loaded from disk
	//it is meant to be a derived, non-persistent value. 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Damage")
		float DamagePerSecond;

	//The UFUNCTION handles the function exposing it to Blueprints
	//UFUNCTION(BlueprintCallable, Category = "Damage")
	//	void CalculateValues();


	//After adding axis mapping to the engine, you have to add this:
	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	//When adding the jump on the engine, remember to put it under action mappings
	UFUNCTION()
		void StartJump();

	UFUNCTION()
		void StopJump();

	// Function that handles firing projectiles.
	UFUNCTION()
		void Fire();

	//FPS Camera. WATCH OUT! Some code has been added to the constructor
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* FPSCameraComponent;

	//Person mesh, visible only to the owning player
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FPSMesh;

	// Gun muzzle's offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile > ProjectileClass;

	// Sets default values for this character's properties
	AMyFirstPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
