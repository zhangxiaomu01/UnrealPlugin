// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "RTReflectionPluginStyle.h"

class FRTReflectionPluginCommands : public TCommands<FRTReflectionPluginCommands>
{
public:

	FRTReflectionPluginCommands()
		: TCommands<FRTReflectionPluginCommands>(TEXT("RTReflectionPlugin"), NSLOCTEXT("Contexts", "RTReflectionPlugin", "RTReflectionPlugin Plugin"), NAME_None, FRTReflectionPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};