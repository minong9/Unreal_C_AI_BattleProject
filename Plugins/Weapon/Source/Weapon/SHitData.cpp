#include "SHitData.h"
#include "SWeaponCheckBoxes.h"
#include "WeaponStyle.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"

TArray<TSharedPtr<class SWeaponCheckBoxes>> SHitData::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SHitData::MakeInstance()
{
	return MakeShareable(new SHitData());
}

TSharedPtr<SWeaponCheckBoxes> SHitData::AddCheckBoxes()
{
	TSharedPtr<SWeaponCheckBoxes> checkBoxes = MakeShareable(new SWeaponCheckBoxes());
	int32 index = CheckBoxes.Add(checkBoxes);

	return CheckBoxes[index];
}

void SHitData::EmptyCheckBoxes()
{
	for (TSharedPtr<SWeaponCheckBoxes> ptr : CheckBoxes)
	{
		if (ptr.IsValid())
			ptr.Reset();
	}

	CheckBoxes.Empty();
}

void SHitData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (SWeaponCheckBoxes::CanDraw(InPropertyHandle, CheckBoxes.Num()))
	{
		int32 index = InPropertyHandle->GetIndexInArray();
		CheckBoxes[index]->SetUtilities(InCustomizationUtils.GetPropertyUtilities());

		FString name = InPropertyHandle->GetPropertyDisplayName().ToString();
		name = "Hit Data - " + name;

		InHeaderRow
			.NameContent()
			[
				SNew(SBorder)
				.BorderImage(FWeaponStyle::Get()->Array_Image.Get())
				[
					InPropertyHandle->CreatePropertyNameWidget(FText::FromString(name))
				]
			]
		.ValueContent()
		//.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
		//.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
		.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
		.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
		[
			CheckBoxes[index]->Draw(true)
		];

		return;
	}

	InHeaderRow
		.NameContent()
		[
			InPropertyHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
		.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
		/*.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
		.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))*/
		[
			InPropertyHandle->CreatePropertyValueWidget()
		];
}

void SHitData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InuCustomizationUtils)
{
	if (SWeaponCheckBoxes::CanDraw(InPropertyHandle, CheckBoxes.Num()))
	{
		int32 index = InPropertyHandle->GetIndexInArray();
		CheckBoxes[index]->DrawProperties(InPropertyHandle, &InChildBuilder);

		return;
	}

	uint32 number = 0;
	InPropertyHandle->GetNumChildren(number);

	for (uint32 i = 0; i < number; i++)
	{
		TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
		IDetailPropertyRow& row = InChildBuilder.AddProperty(handle.ToSharedRef());

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
	}//for(i)
}

