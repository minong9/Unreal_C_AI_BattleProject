#include "SWeaponDetailsView.h"
#include "SWeaponCheckBoxes.h"
#include "SEquipmentData.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailPropertyRow.h"
#include "DetailWidgetRow.h"
#include "Weapons/CWeaponAsset.h"
#include "Animation/AnimMontage.h"

bool SWeaponDetailsView::RefreshByCheckBoxes = false;

TSharedRef<IDetailCustomization> SWeaponDetailsView::MakeInstance()
{

	return MakeShareable(new SWeaponDetailsView());
}

void SWeaponDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type = UCWeaponAsset::StaticClass();

	DetailBuilder.HideCategory("CWeaponAsset");

	//Attachment
	{
		//Editcategory : 있으면 리턴해주고, 없으면 추가해줌
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Attachment", FText::FromName("Attachment"));
		category.AddProperty("AttachmentClass", type);
	}

	//Equipment
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Equipment", FText::FromName("Equipment"));
		category.AddProperty("EquipmentClass", type);

		//수정할 데이터 항목(헤더, 콤보박스ㅁ)을 클릭하면 해당 상세데이터(자식)가 보이게 하기
		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type);

		//체크박스를 다 날리고 새로 만듬
		//기존의 체크 정보는 다 날아가게 되어 있음
		//그래서 Refresh로 실행되었다면 아래 줄은 콜이 안되게 해야함
		if(RefreshByCheckBoxes == false)
		{
			TSharedPtr<SWeaponCheckBoxes> checkBoxes = SEquipmentData::CreateCheckBoxes();
			checkBoxes->AddProperties(row.GetPropertyHandle());

			//기본값 선언
			FEquipmentData data;

			int32 index = 0;
			checkBoxes->CheckDefaultObject(index++, data.Montage);
			checkBoxes->CheckDefaultValue(index++, data.PlayRate);
			checkBoxes->CheckDefaultValue(index++, data.bCanMove);
			checkBoxes->CheckDefaultValue(index++, data.bUseControlRotation);
		}
	}

	//DoActionClass
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("DoAction", FText::FromName("DoAction"));
		IDetailPropertyRow& classRow = category.AddProperty("DoActionClass", type);

		TSharedPtr<SWidget> name;
		TSharedPtr<SWidget> value;

		classRow.GetDefaultWidgets(name, value);

		classRow.CustomWidget()
		.NameContent()
		[
			SNew(STextBlock)
			.TextStyle(FCoreStyle::Get(), "EmphasizedText")
			.ColorAndOpacity(FLinearColor::Yellow)
			.Text(FText::FromString("DoActionClass"))
		]
		.ValueContent()
		.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
		.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
		[
			value.ToSharedRef()
		];

		//DoActionDatas
		{
			IDetailPropertyRow& row = category.AddProperty("DoActionDatas", type);

			if (RefreshByCheckBoxes == false)
			{
				//자식의 자식
				uint32 count = 0;
				row.GetPropertyHandle()->GetNumChildren(count);

				for(uint32 i = 0; i < count; i++)
				{
					TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);

					
				}
			}//if(RefreshByCheckBoxes)
		}
	}
}