// Fill out your copyright notice in the Description page of Project Settings.

#include "MyFirstPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/InputSettings.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Gun.h"

// Sets default values
AMyFirstPlayer::AMyFirstPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set up the components
	FPSSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	FPSSpringArmComponent->SetupAttachment(RootComponent);
	FPSSpringArmComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, BaseEyeHeight), FRotator(-30.0f, 0.0f, 0.0f));
	FPSSpringArmComponent->TargetArmLength = 400.0f;	
	FPSSpringArmComponent->bUsePawnControlRotation = true;
	// Create a first person camera component.	
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	// Attach the camera component to our spring arm	
	FPSCameraComponent->SetupAttachment(FPSSpringArmComponent, USpringArmComponent::SocketName);
	// Position the camera slightly above the eyes.
	// Allow the pawn to control camera rotation.
	FPSCameraComponent->bUsePawnControlRotation = false;	

	HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	HoldingComponent->RelativeLocation.X = 50.0f;
	HoldingComponent->SetupAttachment(GetMesh());

	CurrentItem = NULL;
	bCanMove = true;
	bInspecting = false;	
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

	PitchMax = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax;
	PitchMin = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin;

	if (GunBlueprint == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("The gun has no mesh"));
		return;
	}
	//The GunBlueprint is NULL so it's not working
	Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); //Attach gun mesh
	Gun->AnimInstance = GetMesh()->GetAnimInstance();
	
	//InputComponent->BindAction("Fire", IE_Pressed, Gun, &AGun::OnFire);	
}

// Called every frame
void AMyFirstPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	GetActorEyesViewPoint(CameraLoc, CameraRot);
	Start = CameraLoc;
	ForwardVector = FPSCameraComponent->GetForwardVector();
	End = ((ForwardVector * 200.0f) + Start);

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	if (!bHoldingItem)
	{
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams,
			DefaultResponseParams))
		{
			if (Hit.GetActor()->GetClass()->IsChildOf(APickup::StaticClass()))
			{
				CurrentItem = Cast<APickup>(Hit.GetActor());
			}
		}
		else
		{
			CurrentItem = NULL;
		}
	}	

	if (bInspecting)
	{
		if (bHoldingItem)
		{
			FPSCameraComponent->SetFieldOfView(FMath::Lerp(FPSCameraComponent->FieldOfView,
				90.0f, 0.1f));			 
			HoldingComponent->SetRelativeLocation(FVector(0.0f, -200.0f, 500.0f));
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.9000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -179.9000002f;
			CurrentItem->RotateActor();
		}
		else
		{
			FPSCameraComponent->SetFieldOfView(FMath::Lerp(FPSCameraComponent->FieldOfView,
				45.0f, 0.1f));
		}
	}
	else
	{
		FPSCameraComponent->SetFieldOfView(FMath::Lerp(FPSCameraComponent->FieldOfView,
			90.0f, 0.1f));

		if (bHoldingItem)
		{			
			HoldingComponent->SetRelativeLocation(FVector(0.0f, 100.0f, 0.0f));
		}
	}

	//Check if the player has been hit	
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

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyFirstPlayer::Attack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyFirstPlayer::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyFirstPlayer::StopJump);	

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AMyFirstPlayer::OnAction);
	
	PlayerInputComponent->BindAction("Inspect", IE_Pressed, this, &AMyFirstPlayer::OnInspect);
	PlayerInputComponent->BindAction("Inspect", IE_Released, this, &AMyFirstPlayer::OnInspectReleased);
}

void AMyFirstPlayer::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	if (Value != 0.0f && bCanMove)
	{
		AddMovementInput(Direction, Value);
	}
	
}

void AMyFirstPlayer::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	if (Value != 0.0f && bCanMove)
	{
		AddMovementInput(Direction, Value);
	}
	
}

void AMyFirstPlayer::StartJump()
{
	bPressedJump = true;
}

void AMyFirstPlayer::StopJump()
{
	bPressedJump = false;
}

void AMyFirstPlayer::Attack()
{
	//check for weapon and do different type of attack	
	if(!bHoldingItem)
		{
			Gun->OnFire();
		}	
}


void AMyFirstPlayer::OnAction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("You are pressing E"));
	if (CurrentItem && !bInspecting)
	{
		ToggleItemPickup();
	}
}

void AMyFirstPlayer::OnInspect()
{
	if (bHoldingItem)
	{
		LastRotation = GetControlRotation();
		ToggleMovement();
	}
	else
	{
		bInspecting = true;
	}
}

void AMyFirstPlayer::OnInspectReleased()
{
	if (bInspecting && bHoldingItem)
	{
		GetController()->SetControlRotation(LastRotation);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = PitchMax;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = PitchMin;
		ToggleMovement();
	
	}
	else
	{
		bInspecting = false;
	}
}

void AMyFirstPlayer::ToggleMovement()
{
	bCanMove = !bCanMove;
	bInspecting = !bInspecting;
	FPSCameraComponent->bUsePawnControlRotation = !FPSCameraComponent->bUsePawnControlRotation;
	FPSSpringArmComponent->bUsePawnControlRotation = !FPSSpringArmComponent->bUsePawnControlRotation;
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
}

void AMyFirstPlayer::ToggleItemPickup()
{
	if (CurrentItem)
	{
		bHoldingItem = !bHoldingItem;
		CurrentItem->Pickup();

		if (!bHoldingItem)
		{
			CurrentItem = NULL;
		}
	}
}
