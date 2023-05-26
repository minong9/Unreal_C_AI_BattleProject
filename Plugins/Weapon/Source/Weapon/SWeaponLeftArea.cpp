#include "SWeaponLeftArea.h"
#include "Weapons/CWeaponAsset.h"
#include "EngineUtils.h"
#include "Widgets/Input/SSearchBox.h"

void SWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	Row = InArgs._Row;

	SMultiColumnTableRow<FWeaponRowDataPtr>::Construct
	(
		FSuperRowType::FArguments().Style(FEditorStyle::Get(), "TableView.DarkRow"), InOwnerTable
	);
}

//헤더 갯수에 따라 N번 호출됨
TSharedRef<SWidget> SWeaponTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	FString str;
	if (InColumnName == "Index")				//0번
		str = FString::FromInt(Row->Index);
	else if (InColumnName == "Name")			//1번
		str = Row->Name;

	return SNew(SBox)
	.Padding(4)
	.VAlign(VAlign_Center)
	[
		SNew(STextBlock)
		.Text(FText::FromString(str))
	];


	//return SNew(SVerticalBox)
	//+ SVerticalBox::Slot()
	//.AutoHeight()
	//.Padding(4)
	//.VAlign(VAlign_Center)
	//[
	//	SNew(STextBlock)
	//	.Text(FText::FromString(str))
	//];
}

///////////////////////////////////////////////////////////////////////////////

void SWeaponLeftArea::Construct(const FArguments & InArgs)
{
	OnListViewSelectedItem = InArgs._OnListViewSelectedItem;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2, 0)
		[
			SNew(SBox)
			[
				SAssignNew(SearchBox, SSearchBox)
				.SelectAllTextWhenFocused(true)
				.OnTextChanged(this, &SWeaponLeftArea::OnTextChanged)
				.OnTextCommitted(this, &SWeaponLeftArea::OnTextCommitted)
			]
		]
		+ SVerticalBox::Slot()
		.FillHeight(1)
		[
			SAssignNew(ListView, SWeaponListView)
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column("Index")
				.DefaultLabel(FText::FromString(""))
				.ManualWidth(50)
				+ SHeaderRow::Column("Name")
				.DefaultLabel(FText::FromString("Name"))
			)
			.ListItemsSource(&RowDatas)
			.OnGenerateRow(this, &SWeaponLeftArea::OnGenerateRow)
			.OnSelectionChanged(this, &SWeaponLeftArea::OnSelectionChanged)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(FMargin(8, 2))
			[
				SNew(STextBlock)
				.Text(this, &SWeaponLeftArea::OnGetAssetCount)
			]
		]
	];

	ReadDataAssetList();
}

void SWeaponLeftArea::ReadDataAssetList()
{
	RowDatas.Empty();

	TArray<UObject*> objects;
	EngineUtils::FindOrLoadAssetsByPath("/Game/Weapons/", objects, EngineUtils::ATL_Regular);

	int32 index = 0;
	for (UObject* obj : objects)
	{
		UCWeaponAsset* asset = Cast<UCWeaponAsset>(obj);
		if (asset == nullptr) continue;


		FString name = asset->GetName();
		if (SearchText.IsEmpty() == false)
		{
			if (name.Contains(SearchText.ToString()) == false)
				continue;
		}

		RowDatas.Add(FWeaponRowData::Make(++index, name, asset));
	}

	RowDatas.Sort([](const FWeaponRowDataPtr& A, const FWeaponRowDataPtr& B)
	{
		return A->Index < B->Index;
	});

	ListView->RequestListRefresh();
}

FWeaponRowDataPtr SWeaponLeftArea::GetRowDataPtrByName(FString InName)
{
	if (HasRowPtrs() == false)
		return nullptr;

	for (FWeaponRowDataPtr ptr : RowDatas)
	{
		if (ptr->Name == InName)
			return ptr;
	}

	return nullptr;
}

void SWeaponLeftArea::SelectDataPtr(UCWeaponAsset * InAsset)
{
	if (HasRowPtrs() == false)
		return;

	for (FWeaponRowDataPtr ptr : RowDatas)
	{
		if (ptr->Asset == InAsset)
		{
			ListView->SetSelection(ptr);

			return;
		}
	}
}

FString SWeaponLeftArea::SelectedRowDataPtrName()
{
	TArray<FWeaponRowDataPtr> ptrs = ListView->GetSelectedItems();
	
	if (ptrs.Num() > 0)
		return ptrs[0]->Asset->GetName();

	return "";
}

TSharedRef<ITableRow> SWeaponLeftArea::OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable)
{
	return SNew(SWeaponTableRow, InTable)
		.Row(InRow);
}

void SWeaponLeftArea::OnSelectionChanged(FWeaponRowDataPtr InPtr, ESelectInfo::Type InType)
{
	if (InPtr.IsValid() == false) return;

	//GLog->Log(InPtr->Asset->GetName());

	OnListViewSelectedItem.ExecuteIfBound(InPtr);
}

FText SWeaponLeftArea::OnGetAssetCount() const
{
	FString str = FString::Printf(TEXT("%d 에셋"), RowDatas.Num());

	return FText::FromString(str);
}

void SWeaponLeftArea::OnTextChanged(const FText & InText)
{
	if (SearchText.CompareToCaseIgnored(InText) == 0)
		return;

	SearchText = InText;
	ReadDataAssetList();
}

void SWeaponLeftArea::OnTextCommitted(const FText & InText, ETextCommit::Type InType)
{
	OnTextChanged(InText);
}

//SLATE_ATTRIBUTE
//어트리뷰트 예시) RGB 값 바꾸면 아래 색바뀌는 것
//변수바뀌면 색 바뀌는 이벤트 호출