#include "SWeaponCheckBoxes.h"
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
	{
		InternalDatas.Add(FInternalData(InHandle->GetChildHandle(i)));
	}
}

void SWeaponCheckBoxes::SetUtilities(TSharedPtr<IPropertyUtilities> InUtilities)
{
	Utilities = InUtilities;
}

TSharedRef<SWidget> SWeaponCheckBoxes::Draw()
{
	TSharedPtr<SUniformGridPanel> panel;
	SAssignNew(panel, SUniformGridPanel);
	panel->SetMinDesiredSlotWidth(150);

	for(int32 i = 0; i < InternalDatas.Num(); i++)
	{
		panel->AddSlot(i, 0)
		[
			DrawCheckBox(i)
		];
	}

	return panel.ToSharedRef();
}

void SWeaponCheckBoxes::DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder* InChildBuilder)
{
	for(int32 i = 0; i < InternalDatas.Num();i++)
	{
		if (InternalDatas[i].bChecked == false)
			continue;

		TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);	//번호를 넣으면 해당 것을 리턴해 줌
		//빌더라는 것은 모양을 넣어줄 때 사용함
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
		.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
		.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
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

	//침범형 접근자 정리해두기
	//TSharedPtr<> = MakeShareable(new ~);
	//->Delegate를 주소를 this로 가져오려고 함
	//AsShared : 무조건 부모로 가서 가져오려 해서 에러가 남
	//SharedThis
	//-> 부모와 자식을 맞춰주는 작업을 해야함
	//그래서 TSharedFromThis<SWeaponCheckBoxes> 부모로 설정
}

void SWeaponCheckBoxes::OnCheckStateChanged(ECheckBoxState InState, int32 InIndex)
{
	InternalDatas[InIndex].bChecked = !InternalDatas[InIndex].bChecked;

	SWeaponDetailsView::OnRefreshByCheckBoxes();	//call해줘서 true로 바꿈
	{
		Utilities->ForceRefresh();
	}
	SWeaponDetailsView::OffRefreshByCheckBoxes();	
}

void SWeaponCheckBoxes::CheckDefaultObject(int32 InIndex, UObject* InValue)
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

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, const FVector& InValue)
{
	FVector val;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}


