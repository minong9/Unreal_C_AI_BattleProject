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
		//Editcategory : ������ �������ְ�, ������ �߰�����
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Attachment", FText::FromName("Attachment"));
		category.AddProperty("AttachmentClass", type);
	}

	//Equipment
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Equipment", FText::FromName("Equipment"));
		category.AddProperty("EquipmentClass", type);

		//������ ������ �׸�(���, �޺��ڽ���)�� Ŭ���ϸ� �ش� �󼼵�����(�ڽ�)�� ���̰� �ϱ�
		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type);

		TSharedPtr<SWeaponCheckBoxes> checkBoxes = SEquipmentData::CreateCheckBoxes();
		checkBoxes->AddProperties(row.GetPropertyHandle());

		//�⺻�� ����
		FEquipmentData data;

		int32 index = 0;
		checkBoxes->CheckDefaultObject(index++, data.Montage);
		checkBoxes->CheckDefaultValue(index++, data.PlayRate);
		checkBoxes->CheckDefaultValue(index++, data.bCanMove);
		checkBoxes->CheckDefaultValue(index++, data.bUseControlRotation);
	}
}