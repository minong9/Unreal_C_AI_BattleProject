#include "SEquipmentData.h"

#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"

TSharedRef<IPropertyTypeCustomization> SEquipmentData::MakeInstance()
{
	return MakeShareable(new SEquipmentData());
}

void SEquipmentData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	InHeaderRow
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Name"))
	]
	.ValueContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Value"))
	];
}

//InHeaderRow.GetPropertyHandle[0] == InPropertyHandle : �θ��� ����
void SEquipmentData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InuCustomizationUtils)
{
	uint32 number = 0;
	InPropertyHandle->GetNumChildren(number);

	for(uint32 i = 0; i < number; i++)
	{
		TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);	//��ȣ�� ������ �ش� ���� ������ ��
		//������� ���� ����� �־��� �� �����
		IDetailPropertyRow& row = InChildBuilder.AddProperty(handle.ToSharedRef());

		row.CustomWidget()
		.NameContent()
		[
			
			SNew(STextBlock)
			.Text(handle->GetPropertyDisplayName())
		]
		.ValueContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Value"))
		];
	}
}