#include "WeaponAssetEditor.h"
#include "Weapons/CWeaponAsset.h"
#include "SWeaponListView.h"

const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FWeaponAssetEditor::ListViewTabId = "ListView";
const FName FWeaponAssetEditor::DetailTabId = "Details";

//�ν��Ͻ� �ʱ�ȭ
TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;

//�̱��� ������ ���⼭ �� ����
void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	//â�� �����ִ���?
	//Shutdown�� ������ �ν��Ͻ��� �ȳ��ư��� ��찡 ����
	//�ֳ�? ����� ���������� �ν��Ͻ��� �������� ����
	if(Instance.IsValid())
	{
		// �׷��� ����Ʈ�並 �� �� �� üũ��
		// â�� ����� ������ ����Ʈ��� �������� ����
		// �� �κ��� ���� ������ â�� ���ȴ� ������ �ݺ���. ���� �����ִٸ� �״�� �����͸� �ٲ�� �� ����
		// ����Ʈ�䰡 ���ʷ� �� �� ���ȴٸ�?
		if(Instance->ListView.IsValid())	
		{
			FWeaponRowDataPtr ptr = nullptr;

			if (InAssetName.Len() > 0)	//���� ������ �� �ڷε� �Է� �Ǿ��ٸ�
				ptr = Instance->ListView->GetRowDataPtrByName(InAssetName);

			if (ptr == nullptr)
				ptr = Instance->ListView->GetFirstDataPtr();

			Instance->ListView->SelectDataPtr(ptr->Asset);

			return;
		}

		//ó�� ������ �ƴ϶��
		Instance->CloseWindow();

		Instance.Reset(); //â�� ���������� �ٽ� â�� �ݾ���
		Instance = nullptr;
	}

	Instance = MakeShareable(new FWeaponAssetEditor());
	Instance->Open(InAssetName);
}

void FWeaponAssetEditor::Open(FString InAssetName)
{
	//�Ҵ�, S�� �ٴ� ���� ��� SNew�� �Ҵ�
	//����Ʈ�� ��ü ����
	ListView = SNew(SWeaponListView)
		.OnListViewSelectedItem(this, &FWeaponAssetEditor::OnListViewSelectedItem);

	//�����Ϻ� ��ü ����
	FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs args(false, false, true, FDetailsViewArgs::ObjectsUseNameArea);
	args.ViewIdentifier = "WeaponAssetEditorDetailsView";
	DetailsView = prop.CreateDetailView(args);

	//���� ����
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
				//����Ʈ�� â
				->Split
				(
					FTabManager::NewStack()	//������ ���� ���� �ϳ�
					->SetSizeCoefficient(0.175f)	//������ �ִ� ��
					->AddTab(ListViewTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
				//�����Ϻ� â
				->Split
				(
					FTabManager::NewStack()	
					->SetSizeCoefficient(0.725f)	
					->AddTab(DetailTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
			)
		);

	//���� ������ �Ҵ�
	//UCWeaponAsset* asset = ListView->GetFirstDataPtr()->Asset;
	UCWeaponAsset* asset = nullptr;
	if(InAssetName.Len() > 0)
	{
		FWeaponRowDataPtr ptr = ListView->GetRowDataPtrByName(InAssetName);

		//���� �Լ��� ������ �ϴ� ������ ������ Ȯ��
		if (ListView->SelectedRowDataPtrName() == InAssetName)
			return;

		//������ ������ ó������ ��������
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
	//DetailsView->SetObject(asset);	//�����Ϻ信 ���������� �Ҵ�
	ListView->SelectDataPtr(asset);		//����Ʈ�信�� ���ٱ��� �� ó������

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

	//����Ʈ�� �� ����
	FOnSpawnTab tab;
	tab.BindSP(this, &FWeaponAssetEditor::Spawn_ListViewTab);

	TabManager->RegisterTabSpawner(ListViewTabId, tab);

	//�����Ϻ� �� ����
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

	//�������� �ƴ϶�� �������� ������Ʈ ����
	if (!!GetEditingObject())
		RemoveEditingObject(GetEditingObject());

	//�������� ������Ʈ �Ҵ�
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