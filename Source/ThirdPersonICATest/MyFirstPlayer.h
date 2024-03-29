// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pickup.h"
#include "MyFirstPlayer.generated.h"

UCLASS()
class THIRDPERSONICATEST_API AMyFirstPlayer : public ACharacter
{
	GENERATED_BODY()	

	UPROPERTY(EditAnywhere)
	class USceneComponent* HoldingComponent;		

public:

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

	//FPS Camera. WATCH OUT! Some code has been added to the constructor
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FPSCameraComponent;

	//Spring Arm, consider that it has to be initialised in the constructor
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* FPSSpringArmComponent;

	//Person mesh, visible only to the owning player
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMesh* HeldObjectSlot;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AGun> GunBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class ASword> SwordBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MainHealth = 100.0f;

	/*UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class ATile> TileBlueprint;*/

	// Sets default values for this character's properties
	AMyFirstPlayer();

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	AGun* Gun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	ASword* Sword;

	bool isPaused;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	void OnAction();

	void OnInspect();

	void OnInspectReleased();

	void ToggleMovement();
	
	void ToggleItemPickup();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwapWeapon();

	void PauseMenu();

	virtual void NotifyActorBeginOverlap(AActor * OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	void CauseDamage();

	UPROPERTY(EditAnywhere)
	class APickup* CurrentItem;

	bool bCanMove;

	UPROPERTY(BlueprintReadOnly)
	bool bHoldingItem;
	UPROPERTY(BlueprintReadOnly)
	bool isActorTheSameType;	

	AMyFirstPlayer* PlayerCollided;
	bool bInspecting;

	float PitchMax;
	float PitchMin;

	FVector HoldingComp;
	FRotator LastRotation;
	
	FVector Start;
	FVector ForwardVector;
	FVector End;

	FVector CameraLoc;
	FRotator CameraRot;

	FHitResult Hit;

	FComponentQueryParams DefaultComponentQueryParams;
	FCollisionResponseParams DefaultResponseParams;	

};
