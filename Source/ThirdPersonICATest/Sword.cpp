// Fill out your copyright notice in the Description page of Project Settings.

#include "Sword.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	FP_Sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Sword"));
	FP_Sword->bCastDynamicShadow = false;		// Disallow mesh to cast dynamic shadows
	FP_Sword->CastShadow = false;			// Disallow mesh to cast other shadows

	//TODO:
	//set invisible until player presses the key to change weapon
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASword::OnAttack()
{
	//Make it attack
	// Play a sound if there is one
	if (AttackSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (AttackAnimation != NULL)
	{
		// Get the animation object for the arms mesh		
		/*if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}*/
	}
}

