#include "SWeaponListView.h"
#include "Weapons/CWeaponAsset.h"

void SWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	//�� ���� ����� �������
	Row = InArgs._Row;

	//�θ𿡼� ��Ÿ���� ������ ��
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
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1) //���̰� ȭ�鿡 ������ ����
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
			
			//SNew(STextBlock)
			//.Text(FText::FromString(InArgs._Text))	//�ܺ��� ������Ʈ �������� �ƱԸ�Ʈ�� �ٶ� ���
		]
	];

	//���� ������ ���� �߰�
	RowDatas.Add(FWeaponRowData::Make(1, "Test", nullptr));
	RowDatas.Add(FWeaponRowData::Make(2, "Test1", nullptr));
	RowDatas.Add(FWeaponRowData::Make(3, "Test2", nullptr));
	RowDatas.Add(FWeaponRowData::Make(4, "Test3", nullptr));

	ListViewDatas->RequestListRefresh();
}

TSharedRef<ITableRow> SWeaponListView::OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable)
{
	return SNew(SWeaponTableRow, InTable)
		.Row(InRow);
}
