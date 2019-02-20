// Fill out your copyright notice in the Description page of Project Settings.

#include "MyFirstPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Components/SkeletalMeshComponent.h"
#include "Projectile.h"

// Sets default values
AMyFirstPlayer::AMyFirstPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());
	// Position the camera slightly above the eyes.
	FPSCameraComponent->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
	FPSCameraComponent->SetRelativeLocation(FVector(-200.0f, 0.0f, 150.0f + BaseEyeHeight));
	// Allow the pawn to control camera rotation.
	FPSCameraComponent->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AMyFirstPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine)
	{
		// Put up a debug message for five seconds. The -1 "Key" value (first argument)
		//indicates that we will never need to update or refresh this message.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using MyFirstCharacter."));
	}
}

// Called every frame
void AMyFirstPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyFirstPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyFirstPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyFirstPlayer::MoveRight);
	//Set up "look" bindings. for mouse x the scale is 1.0 and for mouse y it's -1.0
	PlayerInputComponent->BindAxis("Turn", this, &AMyFirstPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyFirstPlayer::AddControllerPitchInput);
	// Set up "action" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyFirstPlayer::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyFirstPlayer::StopJump);

	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyFirstPlayer::Fire);
}

void AMyFirstPlayer::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMyFirstPlayer::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMyFirstPlayer::StartJump()
{
	bPressedJump = true;
}

void AMyFirstPlayer::StopJump()
{
	bPressedJump = false;
}

void AMyFirstPlayer::Fire()
{
	// Attempt to fire a projectile.
	if (ProjectileClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// Skew the aim to be slightly upwards.
		MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// Spawn the projectile at the muzzle.
			AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}
