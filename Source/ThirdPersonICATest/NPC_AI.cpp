// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC_AI.h"

ANPC_AI::ANPC_AI()
{

}

void ANPC_AI::BeginPlay()
{
	Super::BeginPlay();

	if (!ensure(BlackboardToUse)) { return; } // We ensure that pointer isn't null
	UseBlackboard(BlackboardToUse, BB);	
	if (!ensure(BehaviorTreeToUse)) { return; }// We ensure that BehaviorTree isn't null	
	// Run the behavior tree
	RunBehaviorTree(BehaviorTreeToUse);

	// We get the perception component from the components list
	AIPerception = FindComponentByClass<UAIPerceptionComponent>();
	if (!ensure(AIPerception)) { return; }	
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ANPC_AI::SenseSomething);	
	TArray<AActor*> ActorsSensed;
	AIPerception->GetPerceivedActors(HearingSense, ActorsSensed);	

}

// We try to SenseStuff around.
void ANPC_AI::SenseSomething(AActor* UpdatedActor, FAIStimulus Stimulus)
{	
	if (UpdatedActor->ActorHasTag("Enemy") == false)
	{
		// We set Focalpoint if the sense was successfull
		if (Stimulus.WasSuccessfullySensed())
		{
			BB->SetValueAsObject(EnemyKey, UpdatedActor);	
			//override for the door :D
			//door->Lock
		}
		else
		{
			BB->ClearValue(EnemyKey);			
		}
	}
	
}

