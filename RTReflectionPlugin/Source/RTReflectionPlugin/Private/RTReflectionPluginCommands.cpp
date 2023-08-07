// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTReflectionPluginCommands.h"

#define LOCTEXT_NAMESPACE "FRTReflectionPluginModule"

void FRTReflectionPluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "RTReflectionPlugin", "Bring up RTReflectionPlugin window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
