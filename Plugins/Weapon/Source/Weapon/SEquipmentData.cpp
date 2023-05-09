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
		CheckBoxes->Draw()	//콘텐츠 영역에 바로 추가해서 리턴해주려고
	];
}

//InHeaderRow.GetPropertyHandle[0] == InPropertyHandle : 부모의 것임
void SEquipmentData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InuCustomizationUtils)
{
	//uint32 number = 0;
	//InPropertyHandle->GetNumChildren(number);

	//for(uint32 i = 0; i < number; i++)
	//{
	//	TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);	//번호를 넣으면 해당 것을 리턴해 줌
	//	빌더라는 것은 모양을 넣어줄 때 사용함
	//	IDetailPropertyRow& row = InChildBuilder.AddProperty(handle.ToSharedRef());

	//	row.CustomWidget()
	//	.NameContent()
	//	[
	//		
	//		SNew(STextBlock)
	//		.Text(handle->GetPropertyDisplayName())
	//	]
	//	.ValueContent()
	//	[
	//		SNew(STextBlock)
	//		.Text(FText::FromString("Value"))
	//	];
	//}

	//SWeaponCheckBoxes::DrawProperties 안으로 위 내용 옮김
	CheckBoxes->DrawProperties(InPropertyHandle, &InChildBuilder);
}