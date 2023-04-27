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
typedef SListView<FWeaponRowDataPtr> SWeaponListViewRow;	//실제 한줄을 보여줄 객체


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


 

class WEAPON_API SWeaponListView
	: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWeaponListView) {}
	SLATE_ARGUMENT(FString, Text)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	TSharedRef<ITableRow> OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);

private:
	TArray<FWeaponRowDataPtr> RowDatas;
	TSharedPtr<SWeaponListViewRow> ListViewDatas;
};
