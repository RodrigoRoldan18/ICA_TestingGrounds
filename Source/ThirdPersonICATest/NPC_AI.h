// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC_AI.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONICATEST_API ANPC_AI : public AAIController
{
	GENERATED_BODY()


public:
	ANPC_AI();

	UPROPERTY(BlueprintReadOnly)


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	//this will be set up in the BP
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Blackboard")
	UBlackboardData* BlackboardToUse;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	UBehaviorTree* BehaviorTreeToUse;

	//BBC that will be as return value later
	UPROPERTY()
	UBlackboardComponent* BB;

	//Perception component
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerception;

	//EnemyKey component
	UPROPERTY(EditDefaultsOnly)
	FName EnemyKey;

	//Main function, when sensing something.
	UFUNCTION()
	void SenseSomething(AActor* UpdatedActor, FAIStimulus Stimulus);

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	TSubclassOf<UAISense> HearingSense;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	TSubclassOf<UAISense> SightSense;

};


//https://community.gamedev.tv/t/took-me-over-10-hours-to-convert-this-to-c-what-a-learning-progress-i-want-to-share-my-code-with-you-guys-hopefully-it-helps-somebody/23098
