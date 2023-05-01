#include "WeaponContextMenu.h"
#include "WeaponAssetEditor.h"
#include "Weapons/CWeaponAsset.h"

FWeaponContextMenu::FWeaponContextMenu(EAssetTypeCategories::Type InType)
{
	Category = InType;
}

//데이터에셋 파일 클릭해서 여는 것
void FWeaponContextMenu::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	//FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);

	if (InObjects.Num() < 1)
		return;

	FWeaponAssetEditor::OpenWindow(InObjects[0]->GetName());
}

FText FWeaponContextMenu::GetName() const
{
	return FText::FromString("DataAsset");
}

UClass* FWeaponContextMenu::GetSupportedClass() const
{
	return UCWeaponAsset::StaticClass();
}

FColor FWeaponContextMenu::GetTypeColor() const
{
	return FColor::Blue;
}

uint32 FWeaponContextMenu::GetCategories()
{
	return Category;
}
