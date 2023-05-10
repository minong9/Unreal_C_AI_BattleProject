#include "SEquipmentData.h"

#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SWeaponCheckBoxes.h"

TSharedPtr<class SWeaponCheckBoxes> SEquipmentData::CheckBoxes = nullptr;

TSharedRef<IPropertyTypeCustomization> SEquipmentData::MakeInstance()
{
	return MakeShareable(new SEquipmentData());
}

TSharedPtr<SWeaponCheckBoxes> SEquipmentData::CreateCheckBoxes()
{
	if (CheckBoxes.IsValid())
	{
		CheckBoxes.Reset();
		CheckBoxes = nullptr;
	}

	return CheckBoxes = MakeShareable(new SWeaponCheckBoxes());
}

void SEquipmentData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if(CheckBoxes.IsValid() == false)
	{
		//�ذ� ����� üũ�ڽ��� �Ⱦ��� �⺻���� ��
		InHeaderRow
		.NameContent()
		[
			InPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
		.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
		[
			InPropertyHandle->CreatePropertyValueWidget()
		];

		return;
	}

	CheckBoxes->SetUtilities(InCustomizationUtils.GetPropertyUtilities());

	InHeaderRow
	.NameContent()
	[
		InPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
	.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
	[
		CheckBoxes->Draw()	//������ ������ �ٷ� �߰��ؼ� �������ַ���
	];
}

//InHeaderRow.GetPropertyHandle[0] == InPropertyHandle : �θ��� ����
void SEquipmentData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InuCustomizationUtils)
{
	if(CheckBoxes.IsValid() == false)
	{
		uint32 number = 0;
		InPropertyHandle->GetNumChildren(number);

		for(uint32 i = 0; i < number; i++)
		{
			TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);	//��ȣ�� ������ �ش� ���� ������ ��
			//������� ���� ����� �־��� �� �����
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
			.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
			.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
			[
				value.ToSharedRef()
			];
		}//for(i)
		return;
	}

	//SWeaponCheckBoxes::DrawProperties ������ �� ���� �ű�
	CheckBoxes->DrawProperties(InPropertyHandle, &InChildBuilder);
}