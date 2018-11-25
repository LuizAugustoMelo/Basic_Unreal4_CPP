// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Basic_Unreal4_CPPGameMode.h"
#include "Basic_Unreal4_CPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABasic_Unreal4_CPPGameMode::ABasic_Unreal4_CPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
