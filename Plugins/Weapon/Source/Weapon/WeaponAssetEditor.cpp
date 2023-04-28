#include "WeaponAssetEditor.h"
#include "Weapons/CWeaponAsset.h"
#include "SWeaponListView.h"

const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FWeaponAssetEditor::ListViewTabId = "ListView";

//인스턴스 초기화
TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;

//싱글톤 관리를 여기서 할 것임
void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	if(Instance.IsValid())
	{
		Instance->CloseWindow();

		Instance.Reset(); //창이 열려있으면 다시 창을 닫아줌
		Instance = nullptr;
	}

	Instance = MakeShareable(new FWeaponAssetEditor());
	Instance->Open(InAssetName);
}

void FWeaponAssetEditor::Open(FString InAssetName)
{
	//할당, S가 붙는 것은 모두 SNew로 할당
	ListView = SNew(SWeaponListView)
		.OnListViewSelectedItem(this, &FWeaponAssetEditor::OnListViewSelectedItem);

	//툴바 영역
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
					FTabManager::NewStack()	//스택이 들어가면 공간 하나
					->SetSizeCoefficient(0.175f)	//비율로 주는 것
					->AddTab(ListViewTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
			)
		);

	//UObject* asset = NewObject<UObject>();
	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, false, true, NewObject<UCWeaponAsset>());
}

void FWeaponAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab tab;
	tab.BindSP(this, &FWeaponAssetEditor::Spawn_ListViewTab);

	TabManager->RegisterTabSpawner(ListViewTabId, tab);
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_ListViewTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
	[
		ListView.ToSharedRef()
	];
}

void FWeaponAssetEditor::OnListViewSelectedItem(FWeaponRowDataPtr InPtr)
{
	if (InPtr == nullptr) return;

	GLog->Log(FString("Editor : ") + InPtr->Asset->GetName());
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