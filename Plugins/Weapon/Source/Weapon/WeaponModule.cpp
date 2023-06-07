#include "WeaponModule.h" 
#include "WeaponStyle.h"
#include "WeaponCommand.h"
#include "WeaponContextMenu.h"
#include "SEquipmentData.h"
#include "SDoActionData.h"
#include "SHitData.h"

#include "IAssetTools.h"
#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "FWeaponModule"

IMPLEMENT_MODULE(FWeaponModule, Weapon)

void FWeaponModule::StartupModule()
{
	FWeaponStyle::Get();

	Command = MakeShareable(new FWeaponCommand());
	Command->Startup();


	IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type type = assetTools.RegisterAdvancedAssetCategory("WeaponAsset", FText::FromString("Weapon"));

	ContextMenu = MakeShareable(new FWeaponContextMenu(type));
	assetTools.RegisterAssetTypeActions(ContextMenu.ToSharedRef());


	FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
}

void FWeaponModule::ShutdownModule()
{
	if (Command.IsValid())
		Command.Reset();

	if (ContextMenu.IsValid())
		ContextMenu.Reset();

	FWeaponStyle::Shutdown();
}

#undef LOCTEXT_NAMESPACE
