// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonICATestGameMode.h"
#include "ThirdPersonICATestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdPersonICATestGameMode::AThirdPersonICATestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Dynamic/Character/Behaviour/Ue4ASP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
