// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class THIRDPERSONICATEST_API ASword : public AActor
{
	GENERATED_BODY()

	/** Sword mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Sword;
	
public:	
	// Sets default values for this actor's properties
	ASword();

public:	

	//Attacks
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnAttack();

	/** Sound to play each time we attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* AttackSound;

	/** AnimMontage to play each time we attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* AttackAnimation;
	
};
