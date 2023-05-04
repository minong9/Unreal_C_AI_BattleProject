#include "SWeaponDetailsView.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "Weapons/CWeaponAsset.h"

TSharedRef<IDetailCustomization> SWeaponDetailsView::MakeInstance()
{

	return MakeShareable(new SWeaponDetailsView());
}

void SWeaponDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type = UCWeaponAsset::StaticClass();

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
		category.AddProperty("EquipmentData", type);
		
	}
}