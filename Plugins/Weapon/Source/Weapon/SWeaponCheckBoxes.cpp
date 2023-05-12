#include "SWeaponCheckBoxes.h"
#include "WeaponStyle.h"
#include "SWeaponDetailsView.h"

#include "Widgets/Layout/SUniformGridPanel.h"
#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"


void SWeaponCheckBoxes::AddProperties(TSharedPtr<IPropertyHandle> InHandle)
{
	uint32 number = 0;
	InHandle->GetNumChildren(number);

	for (uint32 i = 0; i < number; i++)
		InternalDatas.Add(FInternalData(InHandle->GetChildHandle(i)));
}

void SWeaponCheckBoxes::SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities)
{
	Utilities = InUtilities;
}

TSharedRef<SWidget> SWeaponCheckBoxes::Draw(bool bBackground)
{
	TSharedPtr<SUniformGridPanel> panel;
	SAssignNew(panel, SUniformGridPanel);
	panel->SetMinDesiredSlotWidth(150);

	for (int32 i = 0; i < InternalDatas.Num(); i++)
	{
		panel->AddSlot(i, 0)
		[
			DrawCheckBox(i)
		];
	}

	if (bBackground == false)
		return panel.ToSharedRef();

	TSharedPtr<SBorder> border = SNew(SBorder)
	.BorderImage(FWeaponStyle::Get()->Array_Image.Get())
	[
		panel.ToSharedRef()
	];

	return border.ToSharedRef();
}

void SWeaponCheckBoxes::DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder * InChildBuilder)
{
	for (int32 i = 0; i < InternalDatas.Num(); i++)
	{
		if(InternalDatas[i].bChecked == false)
			continue;


		TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
		IDetailPropertyRow& row = InChildBuilder->AddProperty(handle.ToSharedRef());


		TSharedPtr<SWidget> name;
		TSharedPtr<SWidget> value;

		row.GetDefaultWidgets(name, value);


		row.CustomWidget()
		.NameContent()
		[
			name.ToSharedRef()
		]
		.ValueContent()
		.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
		.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
		//.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
		//.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
		[
			value.ToSharedRef()
		];
	}
}

TSharedRef<SWidget> SWeaponCheckBoxes::DrawCheckBox(int32 InIndex)
{
	return SNew(SCheckBox)
	.IsChecked(InternalDatas[InIndex].bChecked)
	.OnCheckStateChanged(this, &SWeaponCheckBoxes::OnCheckStateChanged, InIndex)
	[
		SNew(STextBlock)
		.Text(FText::FromString(InternalDatas[InIndex].Name))
	];
}

void SWeaponCheckBoxes::OnCheckStateChanged(ECheckBoxState InState, int32 InIndex)
{
	InternalDatas[InIndex].bChecked = !InternalDatas[InIndex].bChecked;

	SWeaponDetailsView::OnRefreshByCheckBoxes();
	{
		Utilities->ForceRefresh();
	}
	SWeaponDetailsView::OffRefreshByCheckBoxes();
}

void SWeaponCheckBoxes::CheckDefaultObject(int32 InIndex, UObject * InValue)
{
	UObject* val = nullptr;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (!!val && InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, float InValue)
{
	float val;
	InternalDatas[InIndex].Handle->GetValue(val);

	if(InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, bool InValue)
{
	bool val;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, const FVector & InValue)
{
	FVector val;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}
