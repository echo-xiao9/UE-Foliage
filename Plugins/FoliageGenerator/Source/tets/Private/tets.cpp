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
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include <Editor/UnrealEd/Public/ObjectTools.h>
#include "ThumbnailRendering/TextureThumbnailRenderer.h"
#include <Editor/UnrealEd/Classes/ThumbnailRendering/ThumbnailManager.h>
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"


static const FName tetsTabName("Generate Thumbnail");



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
		.SetDisplayName(LOCTEXT("FtetsTabTitle", "Generate Thumbnails"))
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

void RenderImage(int32 Width, int32 Height, const uint8* Data,const FString& path, const FString& ImageName, bool bSaveJpgOnly = false)
{
	if (Data)
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		if (ImageWrapper.IsValid())
		{
			ImageWrapper->SetRaw(Data, Width * Height * sizeof(FColor), Width, Height, ERGBFormat::BGRA, 8);
			FString Filename = FString::Printf(TEXT("%s/%s.jpg"),*path, *FPaths::GetBaseFilename(ImageName, true));
			FFileHelper::SaveArrayToFile(ImageWrapper->GetCompressed(100), *Filename);
		}
	}
}

void FtetsModule::PluginButtonClicked()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> AssetDatas;
	ContentBrowserModule.Get().GetSelectedAssets(AssetDatas);
	FString path; //选中文件路径
	FString fileType = TEXT("XmlFile (*.xml)|*.xml"); //过滤文件类型
	FString defaultPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()); //文件选择窗口默认开启路径
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	bool bSuccess = DesktopPlatform->OpenDirectoryDialog(nullptr, TEXT("Choose directory to export thumbnails."), defaultPath,  path);
	if (!bSuccess)
	{
		FText DialogText = FText::Format(
			LOCTEXT("Exporting Failed", "Exporting failed. Something error has happened. Please retry.{0}"), FText::FromString(TEXT(""))
		);
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return;

	}

	for (auto item : AssetDatas) {
		auto AssetData = item.GetAsset();
		if (AssetData)
		{
			FObjectThumbnail ObjThumnail;
			ThumbnailTools::RenderThumbnail(AssetData, 2048, 2048, ThumbnailTools::EThumbnailTextureFlushMode::NeverFlush, nullptr, &ObjThumnail);
			FString imgName = AssetData->GetName();
			RenderImage(ObjThumnail.GetImageWidth(), ObjThumnail.GetImageHeight(), ObjThumnail.AccessImageData().GetData(), path, imgName);
		}
	}
	FText DialogText = FText::Format(
		LOCTEXT("Finish Exporting Thumbnails", "Finish exporting. All the thumbnails can be found in the folder selected.{0}"), FText::FromString(TEXT(""))
	);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
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