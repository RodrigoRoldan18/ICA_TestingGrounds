// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FocusAtActor.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONICATEST_API UFocusAtActor : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual void TickTask(UBehaviorTreeComponent & OwnerComp,uint8 * NodeMemory,float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector FocusActorKey;
	
};

//This is a decorator for the Patrol behaviour. However, the Blackboard doesn't let me select it
//it probably has to be promoted into a BP 