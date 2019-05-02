// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsWithinRange.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONICATEST_API UIsWithinRange : public UBTDecorator
{
	GENERATED_BODY()

	//virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float Range;
	
};

//Same problem as FocusAtActor. UE4 does not let me select it because it probably has
//to be promoted to a BP