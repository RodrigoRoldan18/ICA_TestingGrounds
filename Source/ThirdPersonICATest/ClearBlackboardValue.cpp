// Fill out your copyright notice in the Description page of Project Settings.

#include "ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	BlackboardComp->ClearValue(KeyToClear.SelectedKeyName);	

	return EBTNodeResult::Succeeded;
}
