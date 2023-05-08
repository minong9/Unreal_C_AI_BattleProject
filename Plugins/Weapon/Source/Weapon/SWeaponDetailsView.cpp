#include "SWeaponDetailsView.h"
#include "SWeaponCheckBoxes.h"
#include "SEquipmentData.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "Weapons/CWeaponAsset.h"
#include "Animation/AnimMontage.h"

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