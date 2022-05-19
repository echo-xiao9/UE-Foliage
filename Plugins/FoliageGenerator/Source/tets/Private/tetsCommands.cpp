// Copyright Epic Games, Inc. All Rights Reserved.

#include "tetsCommands.h"

#define LOCTEXT_NAMESPACE "FtetsModule"

void FtetsCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Export Thumbnails", "Click to export thumbnails of assets selected.", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
