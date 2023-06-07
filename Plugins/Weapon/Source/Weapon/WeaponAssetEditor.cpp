#include "WeaponAssetEditor.h"

#include "SDoActionData.h"
#include "SEquipmentData.h"
#include "SHitData.h"
#include "Weapons/CWeaponAsset.h"
#include "SWeaponLeftArea.h"
#include "SWeaponDetailsView.h"

const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FWeaponAssetEditor::LeftAreaTabId = "LeftArea";
const FName FWeaponAssetEditor::DetailTabId = "Details";

TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;



void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	if (Instance.IsValid())
	{
		if (Instance->LeftArea.IsValid())
		{
			FWeaponRowDataPtr ptr = nullptr;

			if (InAssetName.Len() > 0)
				ptr = Instance->LeftArea->GetRowDataPtrByName(InAssetName);

			if (ptr == nullptr)
				ptr = Instance->LeftArea->GetFirstDataPtr();

			Instance->LeftArea->SelectDataPtr(ptr->Asset);

			return;
		}

		Instance->CloseWindow();

		Instance.Reset();
		Instance = nullptr;
	}

	Instance = MakeShareable(new FWeaponAssetEditor());
	Instance->Open(InAssetName);
}

void FWeaponAssetEditor::Shutdown()
{
	if (Instance.IsValid())
	{
		Instance->CloseWindow();

		Instance.Reset();
		Instance = nullptr;
	}
}

void FWeaponAssetEditor::Open(FString InAssetName)
{
	LeftArea = SNew(SWeaponLeftArea)
		.OnListViewSelectedItem(this, &FWeaponAssetEditor::OnListViewSelectedItem);

	
	FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//DetatilsView Customization
	{
		FDetailsViewArgs args(false, false, true, FDetailsViewArgs::ObjectsUseNameArea);
		args.ViewIdentifier = "WeaponAssetEditorDetailsView";
		DetailsView = prop.CreateDetailView(args);

		FOnGetDetailCustomizationInstance detailsView = FOnGetDetailCustomizationInstance::CreateStatic(&SWeaponDetailsView::MakeInstance);
		DetailsView->SetGenericLayoutDetailsDelegate(detailsView);
	}

	//PropertyType Customization
	{
		prop.RegisterCustomPropertyTypeLayout("EquipmentData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SEquipmentData::MakeInstance));
		prop.RegisterCustomPropertyTypeLayout("DoActionData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SDoActionData::MakeInstance));
		prop.RegisterCustomPropertyTypeLayout("HitData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&SHitData::MakeInstance));
	}



	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("WeaponAssetEditor_Layout")
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
		)
		->Split
		(
			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.175f)
				->AddTab(LeftAreaTabId, ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.725f)
				->AddTab(DetailTabId, ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
		)
	);
	

	//UCWeaponAsset* asset = ListView->GetFirstDataPtr()->Asset;

	UCWeaponAsset* asset = nullptr;
	if (InAssetName.Len() > 0)
	{
		FWeaponRowDataPtr ptr = LeftArea->GetRowDataPtrByName(InAssetName);

		if (LeftArea->SelectedRowDataPtrName() == InAssetName)
			return;

		if (ptr.IsValid() == false)
			asset = LeftArea->GetFirstDataPtr()->Asset;
		else
			asset = ptr->Asset;
	}
	else
	{
		asset = LeftArea->GetFirstDataPtr()->Asset;
	}

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, false, true, asset);

	//DetailsView->SetObject(asset);
	LeftArea->SelectDataPtr(asset);
}

bool FWeaponAssetEditor::OnRequestClose()
{
	if (!!DetailsView)
	{
		if (!!GEditor && !!GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this);

		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			prop.UnregisterCustomPropertyTypeLayout("EquipmentData");
			prop.UnregisterCustomPropertyTypeLayout("DoActionData");
			prop.UnregisterCustomPropertyTypeLayout("HitData");
		}
	}

	if (LeftArea.IsValid())
		LeftArea.Reset();

	if (DetailsView.IsValid())
		DetailsView.Reset();

	return true;
}

void FWeaponAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab tab;
	tab.BindSP(this, &FWeaponAssetEditor::Spawn_LeftAreaTab);

	TabManager->RegisterTabSpawner(LeftAreaTabId, tab);

	
	FOnSpawnTab tab2;
	tab2.BindSP(this, &FWeaponAssetEditor::Spawn_DetailsViewTab);

	TabManager->RegisterTabSpawner(DetailTabId, tab2);
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_LeftAreaTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
	[
		LeftArea.ToSharedRef()
	];
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_DetailsViewTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
	[
		DetailsView.ToSharedRef()
	];
}

void FWeaponAssetEditor::OnListViewSelectedItem(FWeaponRowDataPtr InPtr)
{
	if (InPtr == nullptr) return;

	if (!!GetEditingObject())
		RemoveEditingObject(GetEditingObject());

	AddEditingObject(InPtr->Asset);
	DetailsView->SetObject(InPtr->Asset);
}

FName FWeaponAssetEditor::GetToolkitFName() const
{
	return EditorName;
}

FText FWeaponAssetEditor::GetBaseToolkitName() const
{
	return FText::FromName(EditorName);
}

FString FWeaponAssetEditor::GetWorldCentricTabPrefix() const
{
	return EditorName.ToString();
}

FLinearColor FWeaponAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0, 0, 1);
}