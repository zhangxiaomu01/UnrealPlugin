// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTReflectionPlugin.h"
#include "RTReflectionPluginStyle.h"
#include "RTReflectionPluginCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "SRelectionPluginMainWidget.h"

static const FName RTReflectionPluginTabName("RTReflectionPlugin");

#define LOCTEXT_NAMESPACE "FRTReflectionPluginModule"

void FRTReflectionPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FRTReflectionPluginStyle::Initialize();
	FRTReflectionPluginStyle::ReloadTextures();

	FRTReflectionPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FRTReflectionPluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FRTReflectionPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FRTReflectionPluginModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(RTReflectionPluginTabName, FOnSpawnTab::CreateRaw(this, &FRTReflectionPluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FRTReflectionPluginTabTitle", "RTReflectionPlugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FRTReflectionPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FRTReflectionPluginStyle::Shutdown();

	FRTReflectionPluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(RTReflectionPluginTabName);
}

TSharedRef<SDockTab> FRTReflectionPluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SRelectionPluginMainWidget)
		];
}

void FRTReflectionPluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(RTReflectionPluginTabName);
}

void FRTReflectionPluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FRTReflectionPluginCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FRTReflectionPluginCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRTReflectionPluginModule, RTReflectionPlugin)