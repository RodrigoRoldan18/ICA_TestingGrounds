// Fill out your copyright notice in the Description page of Project Settings.

#include "FocusAtActor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UFocusAtActor::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();	
	auto Object = BlackboardComp->GetValueAsObject(FocusActorKey.SelectedKeyName);
	AIController->SetFocus(Cast<AActor>(Object));
	
}
