#include "SWeaponListView.h"
#include "Weapons/CWeaponAsset.h"
#include "EngineUtils.h" //엔진 기능들을 제공해주기 위한 헤더
#include "Widgets/Input/SSearchBox.h"

void SWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	//한 줄의 모양을 만들어줌
	Row = InArgs._Row;

	//부모에서 스타일을 지정해 줌
	SMultiColumnTableRow<FWeaponRowDataPtr>::Construct
	(
		FSuperRowType::FArguments().Style(FEditorStyle::Get(), "TableView.DarkRow"), InOwnerTable
	);
}

TSharedRef<SWidget> SWeaponTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	FString str;
	if (InColumnName == "Index")
		str = FString::FromInt(Row->Index);
	else if (InColumnName == "Name")
		str = Row->Name;

	return SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	.AutoHeight()
	.Padding(4)
	.VAlign(VAlign_Center)
	[
		SNew(STextBlock)
		.Text(FText::FromString(str))
	];
}

///////////////////////////////////////////////////////////////////////////////

void SWeaponListView::Construct(const FArguments& InArgs)
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
				.OnTextChanged(this, &SWeaponListView::OnTextChanged)
				.OnTextCommitted(this, &SWeaponListView::OnTextCommitted)
			]
		]
		+ SVerticalBox::Slot()
		.FillHeight(1) //높이가 화면에 꽉차게 설정
		[
			SAssignNew(ListViewDatas, SWeaponListViewRow)
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
			.OnGenerateRow(this, &SWeaponListView::OnGenerateRow)
			.OnSelectionChanged(this, &SWeaponListView::OnSelectionChanged)

			//SNew(STextBlock)
			//.Text(FText::FromString(InArgs._Text))	//외부의 슬레이트 문법에서 아규먼트를 줄때 사용
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)	//오른쪽 정렬
			.Padding(FMargin(8, 2))
			[
				SNew(STextBlock)
				//슬레이트는 안에 이벤트를 넣어줄 수도 있음, 고정된 값말고도 필요할 수 있기 때문
				//Argument 변수만 / Event 이벤트만/ Attribute 이벤트 + 변수 / 
				.Text(this, &SWeaponListView::OnGetAssetCount)
			]
		]
	];

	//실제 데이터 추가
	ReadDataAssetList();
}

void SWeaponListView::ReadDataAssetList()
{
	RowDatas.Empty();

	TArray<UObject*> objects;
	//모든 에셋들을 찾아주는 함수 FindOrLoadAssetsByPath
	EngineUtils::FindOrLoadAssetsByPath("/Game/Weapons/", objects, EngineUtils::ATL_Regular);

	//그 중에서 UCWeaponAsset에 해당하는 데이터만 찾음
	int32 index = 0;
	for(UObject* obj : objects)
	{
		UCWeaponAsset* asset = Cast<UCWeaponAsset>(obj);
		if (asset == nullptr) continue;

		FString name = asset->GetName();
		if(SearchText.IsEmpty() == false)
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

	ListViewDatas->RequestListRefresh();
}

FWeaponRowDataPtr SWeaponListView::GetRowDataPtrByName(FString InName)
{
	if (HasRowPtrs() == false)
		return nullptr;

	for(FWeaponRowDataPtr ptr : RowDatas)
	{
		if (ptr->Name == InName)
			return ptr;
	}

	return nullptr;
}

void SWeaponListView::SelectDataPtr(UCWeaponAsset* InAsset)
{
	if (HasRowPtrs() == false)
		return;

	for(FWeaponRowDataPtr ptr : RowDatas)
	{
		if(ptr->Asset == InAsset)
		{
			ListViewDatas->SetSelection(ptr);	//아이템을 바꾸고 OnSelectionChanged를 콜해줌

			return;
		}
	}
}

FString SWeaponListView::SelectedRowDataPtrName()
{
	TArray<FWeaponRowDataPtr> ptrs = ListViewDatas->GetSelectedItems();

	if (ptrs.Num() > 0)
		return ptrs[0]->Asset->GetName();

	return "";
}

TSharedRef<ITableRow> SWeaponListView::OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable)
{
	return SNew(SWeaponTableRow, InTable)
		.Row(InRow);
}

void SWeaponListView::OnSelectionChanged(FWeaponRowDataPtr InPtr, ESelectInfo::Type InType)
{
	if (InPtr.IsValid() == false) return;

	//GLog->Log(InPtr->Asset->GetName());

	//선택된 개체를 반환해줌. 선택된 개체를 웨폰에셋에서 다뤄야하기 때문
	OnListViewSelectedItem.ExecuteIfBound(InPtr);
}

FText SWeaponListView::OnGetAssetCount() const
{
	FString str = FString::Printf(TEXT("%d 에셋"), RowDatas.Num());

	return FText::FromString(str);

}

void SWeaponListView::OnTextChanged(const FText& InText)
{
	//대소문자(Case)와 상관없이 문자를 비교함
	if (SearchText.CompareToCaseIgnored(InText) == 0)	//Compare는 -1, 0, 1로 반환함
		return;

	//실제 출력되야하는 건 숫자가 아니라 SearchText
	SearchText = InText;
	ReadDataAssetList();
}

void SWeaponListView::OnTextCommitted(const FText& InText, ETextCommit::Type InType)
{
	OnTextChanged(InText);
}
