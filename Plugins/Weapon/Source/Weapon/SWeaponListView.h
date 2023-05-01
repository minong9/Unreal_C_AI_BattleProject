#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STableRow.h"

struct FWeaponRowData
{
	int Index;
	FString Name;
	class UCWeaponAsset* Asset;

	FWeaponRowData()
	{
		
	}

	FWeaponRowData(int32 InIndex, FString InName, class UCWeaponAsset* InAsset)
		: Index(InIndex), Name(InName), Asset(InAsset)
	{
		
	}

	static TSharedPtr<FWeaponRowData> Make(int32 InIndex, FString InName, class UCWeaponAsset* InAsset)
	{
		return MakeShareable(new FWeaponRowData(InIndex, InName, InAsset));
	}
};

typedef TSharedPtr<FWeaponRowData> FWeaponRowDataPtr;
typedef SListView<FWeaponRowDataPtr> SWeaponListViewRow;	//���� ������ ������ ��ü


///////////////////////////////////////////////////////////////////////////////


class WEAPON_API SWeaponTableRow
	: public SMultiColumnTableRow<FWeaponRowDataPtr>
{
public:
	SLATE_BEGIN_ARGS(SWeaponTableRow) {}
	SLATE_ARGUMENT(FWeaponRowDataPtr, Row)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable);

protected:
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:
	FWeaponRowDataPtr Row;
};


///////////////////////////////////////////////////////////////////////////////


DECLARE_DELEGATE_OneParam(FOnWeaponListViewSelectedItem, FWeaponRowDataPtr);

class WEAPON_API SWeaponListView
	: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWeaponListView) {}
	SLATE_EVENT(FOnWeaponListViewSelectedItem, OnListViewSelectedItem)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	void ReadDataAssetList();

public:
	bool HasRowPtrs() { return RowDatas.Num() > 0; }
	FWeaponRowDataPtr GetFirstDataPtr() { return RowDatas[0]; }

	FWeaponRowDataPtr GetRowDataPtrByName(FString InName);
	void SelectDataPtr(class UCWeaponAsset* InAsset);

	FString SelectedRowDataPtrName();

private:
	TSharedRef<ITableRow> OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);
	void OnSelectionChanged(FWeaponRowDataPtr InPtr, ESelectInfo::Type InType);

	FText OnGetAssetCount() const;

	void OnTextChanged(const FText& InText);
	void OnTextCommitted(const FText& InText, ETextCommit::Type InType);

private:
	FOnWeaponListViewSelectedItem OnListViewSelectedItem;

private:
	TArray<FWeaponRowDataPtr> RowDatas;
	TSharedPtr<SWeaponListViewRow> ListViewDatas;

private:
	TSharedPtr<class SSearchBox> SearchBox;
	FText SearchText;
};