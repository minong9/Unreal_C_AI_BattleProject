#include "WeaponAssetEditor.h"
#include "Weapons/CWeaponAsset.h"
#include "SWeaponListView.h"

const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FWeaponAssetEditor::ListViewTabId = "ListView";
const FName FWeaponAssetEditor::DetailTabId = "Details";

//인스턴스 초기화
TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;

//싱글톤 관리를 여기서 할 것임
void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	//창이 열려있느냐?
	//Shutdown을 했지만 인스턴스가 안날아가는 경우가 있음
	//왜냐? 모듈이 끝날때까지 인스턴스가 지워지지 않음
	if(Instance.IsValid())
	{
		// 그래서 리스트뷰를 한 번 더 체크함
		// 창을 지우면 무조건 리스트뷰는 없어지기 때문
		// 이 부분을 넣지 않으면 창이 열렸다 켜졌다 반복됨. 실제 열려있다면 그대로 데이터만 바뀌는 게 맞음
		// 리스트뷰가 최초로 한 번 열렸다면?
		if(Instance->ListView.IsValid())	
		{
			FWeaponRowDataPtr ptr = nullptr;

			if (InAssetName.Len() > 0)	//에셋 네임이 한 자로도 입력 되었다면
				ptr = Instance->ListView->GetRowDataPtrByName(InAssetName);

			if (ptr == nullptr)
				ptr = Instance->ListView->GetFirstDataPtr();

			Instance->ListView->SelectDataPtr(ptr->Asset);

			return;
		}

		//처음 열린게 아니라면
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
	//리스트뷰 객체 생성
	ListView = SNew(SWeaponListView)
		.OnListViewSelectedItem(this, &FWeaponAssetEditor::OnListViewSelectedItem);

	//디테일뷰 객체 생성
	FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs args(false, false, true, FDetailsViewArgs::ObjectsUseNameArea);
	args.ViewIdentifier = "WeaponAssetEditorDetailsView";
	DetailsView = prop.CreateDetailView(args);

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
				//리스트뷰 창
				->Split
				(
					FTabManager::NewStack()	//스택이 들어가면 공간 하나
					->SetSizeCoefficient(0.175f)	//비율로 주는 것
					->AddTab(ListViewTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
				//디테일뷰 창
				->Split
				(
					FTabManager::NewStack()	
					->SetSizeCoefficient(0.725f)	
					->AddTab(DetailTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
			)
		);

	//실제 데이터 할당
	//UCWeaponAsset* asset = ListView->GetFirstDataPtr()->Asset;
	UCWeaponAsset* asset = nullptr;
	if(InAssetName.Len() > 0)
	{
		FWeaponRowDataPtr ptr = ListView->GetRowDataPtrByName(InAssetName);

		//열린 함수와 열려고 하는 파일이 같은지 확인
		if (ListView->SelectedRowDataPtrName() == InAssetName)
			return;

		//파일이 없으면 처음으로 보내버림
		if (ptr.IsValid() == false)
			asset = ListView->GetFirstDataPtr()->Asset;
		else
			asset = ptr->Asset;
	}
	else
	{
		asset = ListView->GetFirstDataPtr()->Asset;
	}

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, false, true, asset);
	//DetailsView->SetObject(asset);	//디테일뷰에 실제데이터 할당
	ListView->SelectDataPtr(asset);		//리스트뷰에서 윗줄까지 다 처리해줌

}

bool FWeaponAssetEditor::OnRequestClose()
{
	if (!!DetailsView)
	{
		if (!!GEditor && !!GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this);
	}

	if (ListView.IsValid())
		ListView.Reset();

	if (DetailsView.IsValid())
		DetailsView.Reset();

	return true;
}

void FWeaponAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	//리스트뷰 탭 생성
	FOnSpawnTab tab;
	tab.BindSP(this, &FWeaponAssetEditor::Spawn_ListViewTab);

	TabManager->RegisterTabSpawner(ListViewTabId, tab);

	//디테일뷰 탭 생성
	FOnSpawnTab tab2;
	tab.BindSP(this, &FWeaponAssetEditor::Spawn_DetailsViewTab);

	TabManager->RegisterTabSpawner(DetailTabId, tab);
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_ListViewTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
	[
		ListView.ToSharedRef()
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

	//GLog->Log(FString("Editor : ") + InPtr->Asset->GetName());

	//편집중이 아니라면 편집중인 오브젝트 해제
	if (!!GetEditingObject())
		RemoveEditingObject(GetEditingObject());

	//편집중인 오브젝트 할당
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