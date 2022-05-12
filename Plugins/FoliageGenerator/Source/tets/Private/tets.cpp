// Copyright Epic Games, Inc. All Rights Reserved.

#include "tets.h"
#include "tetsStyle.h"
#include "tetsCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "PlantBunchManager.h"
#include "PlantManagerDetailPanel.h"

static const FName tetsTabName("tets");



void FtetsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FtetsStyle::Initialize();
	FtetsStyle::ReloadTextures();

	FtetsCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FtetsCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FtetsModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FtetsModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(tetsTabName, FOnSpawnTab::CreateRaw(this, &FtetsModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FtetsTabTitle", "tets"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	{
        static FName PropertyEditor("PropertyEditor");
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
        PropertyModule.RegisterCustomClassLayout(APlantBunchManager::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FPlantManagerDetailPanel::MakeInstance));
    }

}

void FtetsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FtetsStyle::Shutdown();

	FtetsCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(tetsTabName);
}

TSharedRef<SDockTab> FtetsModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add coaaade to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FtetsModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("tetsaaa.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SButton).Text(WidgetText)
			]
		];
}

void FtetsModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(tetsTabName);
}

void FtetsModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FtetsCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FtetsCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FtetsModule, tets)