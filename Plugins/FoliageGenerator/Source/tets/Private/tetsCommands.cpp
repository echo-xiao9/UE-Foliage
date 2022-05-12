// Copyright Epic Games, Inc. All Rights Reserved.

#include "tetsCommands.h"

#define LOCTEXT_NAMESPACE "FtetsModule"

void FtetsCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "tets", "Bring up tets window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
