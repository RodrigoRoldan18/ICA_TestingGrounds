// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonICATestCharacter.h"
#include "PatrollingGuard.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONICATEST_API APatrollingGuard : public AThirdPersonICATestCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "Patrol Route")
	TArray<AActor*> PatrolPointsCPP;

};
