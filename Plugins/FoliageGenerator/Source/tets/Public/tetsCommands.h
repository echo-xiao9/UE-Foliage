// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "tetsStyle.h"

class FtetsCommands : public TCommands<FtetsCommands>
{
public:

	FtetsCommands()
		: TCommands<FtetsCommands>(TEXT("tets"), NSLOCTEXT("Contexts", "tets", "tets Plugin"), NAME_None, FtetsStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};