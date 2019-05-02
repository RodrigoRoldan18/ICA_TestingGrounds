// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "ActorPool.h"
#include "MyFirstPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Navigation/NavigationSystem.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NavigationBoundsOffset = FVector(2000, 0, 0);
	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);
	MinEnemies = 1;
	MaxEnemies = 10;

	BarrierMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrierCode"));
	BarrierMesh->SetupAttachment(RootComponent);
	
}

void ATile::SetPool(UActorPool * InPool)
{
	//the pool of navMeshes is obtained
	Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	//if it has any navmeshes then it will get one navmesh and set it in the world
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == nullptr)
	{
		return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();

}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	//depending on what type, the actor will be placed with specific components.
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		bool found = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);
		if (found)
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{		
	//Spawns the assets into the level
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	//spawns the ai into the level
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1, 1);
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	//it uses a trial and error method where it tries to spawn the object on the map
	FBox Bounds(MinExtent, MaxExtent);
	const int MAX_ATTEMPTS = 100;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OutLocation = CandidatePoint;
			return true;
		}
	}
	return false;
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition& SpawnPosition)
{
	//after the location has been confirmed, it spawns the actor in the level
	FRotator Rotation = FRotator(0, SpawnPosition.Rotation, 0);
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPosition.Location, Rotation);
	if (Spawned != nullptr)
	{		
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));		
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Enemy"));
	}	
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition)
{
	//after the location has been confirmed, it spawns the actor in the level
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (Spawned != nullptr)
	{
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	}	
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();	

	LockGate();
	if (Rock1BP == NULL) { return; }
	PlaceActors(Rock1BP, 1, 4, 150.0f, 1.0f, 5.0f);
	if (Rock5BP == NULL) { return; }	
	PlaceActors(Rock5BP, 1, 4, 250.0f, 1.0f, 5.0f);
	if (Rock17BP == NULL) { return; }
	PlaceActors(Rock17BP, 1, 4, 250.0f, 1.0f, 5.0f);
	if (AIBP == NULL) { return; }
	PlaceAIPawns(AIBP, MinEnemies, MaxEnemies, 100.0f);

}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Pool != nullptr && NavMeshBoundsVolume != nullptr)
	{
		Pool->Return(NavMeshBoundsVolume);
		//remove actors here for garbage collection
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//it locks the gates behind the player
	APawn* HumanPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AMyFirstPlayer* ConfirmedPlayer = Cast<AMyFirstPlayer>(HumanPlayer);
	if (!ensure(ConfirmedPlayer)) { return; }
	if (BarrierMesh->GetComponentLocation().X < ConfirmedPlayer->GetActorLocation().X)
	{
		LockGate();
	}	
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	//creates a sphere cast that will be used to determine if it is colliding with any other objects
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));
	return !HasHit;
}

void ATile::UnlockGate()
{	
	//mechanic to open the gate
	if (BarrierMesh == nullptr || BarrierGreen == nullptr) { return; }
	BarrierMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BarrierMesh->SetMaterial(0, BarrierGreen);
}

void ATile::LockGate()
{	
	//mechanic to close the gate
	if (BarrierMesh == nullptr || BarrierRed == nullptr) { return; }
	BarrierMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BarrierMesh->SetMaterial(0, BarrierRed);
}

FVector ATile::GetGateLocation()
{
	//gives the location of the gate in the level
	return BarrierMesh->GetComponentLocation();
}