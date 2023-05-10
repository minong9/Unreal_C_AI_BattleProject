#include "SWeaponDetailsView.h"
#include "SWeaponCheckBoxes.h"
#include "SEquipmentData.h"
#include "SDoActionData.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailPropertyRow.h"
#include "DetailWidgetRow.h"
#include "SHitData.h"
#include "Weapons/CWeaponAsset.h"
#include "Animation/AnimMontage.h"
#include "Widgets/Docking/SDockTabStack.h"

//bool SWeaponDetailsView::RefreshByCheckBoxes = false;
//
//TSharedRef<IDetailCustomization> SWeaponDetailsView::MakeInstance()
//{
//
//	return MakeShareable(new SWeaponDetailsView());
//}
//
//void SWeaponDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
//{
//	UClass* type = UCWeaponAsset::StaticClass();
//
//	DetailBuilder.HideCategory("CWeaponAsset");
//
//	//Attachment
//	{
//		//Editcategory : ������ �������ְ�, ������ �߰�����
//		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("ClassSettings", FText::FromName("Class Settings"));
//		category.AddProperty("AttachmentClass", type);
//		category.AddProperty("EquipmentClass", type);
//		category.AddProperty("DoActionClass", type);
//	}
//
//	//Equipment
//	{
//		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Equipment", FText::FromName("Equipment"));
//
//		//������ ������ �׸�(���, �޺��ڽ���)�� Ŭ���ϸ� �ش� �󼼵�����(�ڽ�)�� ���̰� �ϱ�
//		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type);
//
//		//üũ�ڽ��� �� ������ ���� ����
//		//������ üũ ������ �� ���ư��� �Ǿ� ����
//		//�׷��� Refresh�� ����Ǿ��ٸ� �Ʒ� ���� ���� �ȵǰ� �ؾ���
//		if(RefreshByCheckBoxes == false)
//		{
//			TSharedPtr<SWeaponCheckBoxes> checkBoxes = SEquipmentData::CreateCheckBoxes();
//			checkBoxes->AddProperties(row.GetPropertyHandle());
//
//			//�⺻�� ����
//			FEquipmentData data;
//
//			int32 index = 0;
//			checkBoxes->CheckDefaultObject(index++, data.Montage);
//			checkBoxes->CheckDefaultValue(index++, data.PlayRate);
//			checkBoxes->CheckDefaultValue(index++, data.bCanMove);
//			checkBoxes->CheckDefaultValue(index++, data.bUseControlRotation);
//		}
//	}
//
//	//DoActionClass
//	{
//		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("DoAction", FText::FromName("DoAction"));
//		IDetailPropertyRow& classRow = category.AddProperty("DoActionClass", type);
//	}
//
//	//DoActionDatas
//	{
//		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("DoActionData", FText::FromName("DoActionData"));
//		IDetailPropertyRow& row = category.AddProperty("DoActionDatas", type);
//
//		if (RefreshByCheckBoxes == false)
//		{
//			//�ڽ��� �ڽ�
//			uint32 count = 0;
//			row.GetPropertyHandle()->GetNumChildren(count);
//
//			SDoActionData::EmptyCheckBoxes();
//
//			FDoActionData data;
//			for(uint32 i = 0; i < count; i++)
//			{
//				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);
//
//				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SDoActionData::AddCheckBoxes();
//				checkBoxes->AddProperties(handle);
//
//				//�⺻��
//				int32 index = 0;
//				checkBoxes->CheckDefaultObject(index++, data.Montage);
//				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
//				checkBoxes->CheckDefaultValue(index++, data.bCanMove);
//				checkBoxes->CheckDefaultValue(index++, data.bFixedCamera);
//				//checkBoxes->CheckDefaultValue(index++, data.Effect);
//				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
//				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
//
//			}
//		}//if(RefreshByCheckBoxes)
//	}
//
//	//HitData
//	{
//		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("HitData", FText::FromName("HitData"));
//		IDetailPropertyRow& row = category.AddProperty("HitDatas", type);
//
//		if (RefreshByCheckBoxes == false)
//		{
//			//�ڽ��� �ڽ�
//			uint32 count = 0;
//			row.GetPropertyHandle()->GetNumChildren(count);
//
//			SHitData::EmptyCheckBoxes();
//
//			FHitData data;
//			for (uint32 i = 0; i < count; i++)
//			{
//				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);
//
//				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SHitData::AddCheckBoxes();
//				checkBoxes->AddProperties(handle);
//
//				//�⺻��
//				int32 index = 0;
//				checkBoxes->CheckDefaultObject(index++, data.Montage);
//				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
//				checkBoxes->CheckDefaultValue(index++, data.bCanMove);
//				checkBoxes->CheckDefaultValue(index++, data.Power);
//				checkBoxes->CheckDefaultValue(index++, data.Launch);
//				checkBoxes->CheckDefaultValue(index++, data.StopTime);
//				//checkBoxes->CheckDefaultValue(index++, data.Sound);
//				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
//				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
//
//			}
//		}//if(RefreshByCheckBoxes)
//	}
//	
//}

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
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("ClassSettings", FText::FromName("Class Settings"));
		category.AddProperty("AttachmentClass", type);
		category.AddProperty("EquipmentClass", type);
		category.AddProperty("DoActionClass", type);
	}

	//Equipment
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Equipment", FText::FromName("Equipment"));
		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type);

		if (RefreshByCheckBoxes == false)
		{
			TSharedPtr<SWeaponCheckBoxes> checkBoxes = SEquipmentData::CreateCheckBoxes();
			checkBoxes->AddProperties(row.GetPropertyHandle());


			FEquipmentData data;

			int32 index = 0;
			checkBoxes->CheckDefaultObject(index++, data.Montage);
			checkBoxes->CheckDefaultValue(index++, data.PlayRate);
			checkBoxes->CheckDefaultValue(index++, data.bCanMove);
			checkBoxes->CheckDefaultValue(index++, data.bUseControlRotation);
		}
	}

	//DoActionDatas
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("DoActionData", FText::FromName("DoActionData"));
		IDetailPropertyRow& row = category.AddProperty("DoActionDatas", type);

		if (RefreshByCheckBoxes == false)
		{
			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);

			SDoActionData::EmptyCheckBoxes();

			FDoActionData data;
			for (uint32 i = 0; i < count; i++)
			{
				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);

				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SDoActionData::AddCheckBoxes();
				checkBoxes->AddProperties(handle);


				int32 index = 0;
				checkBoxes->CheckDefaultObject(index++, data.Montage);
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.bCanMove);
				checkBoxes->CheckDefaultValue(index++, data.bFixedCamera);
				//checkBoxes->CheckDefaultValue(index++, data.Effect);
				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
			}
		}//if(RefreshByCheckBoxes)
	}

	//HitData
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("HitData", FText::FromName("HitData"));
		IDetailPropertyRow& row = category.AddProperty("HitDatas", type);

		if (RefreshByCheckBoxes == false)
		{
			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);

			SHitData::EmptyCheckBoxes();

			FHitData data;
			for (uint32 i = 0; i < count; i++)
			{
				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);

				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SHitData::AddCheckBoxes();
				checkBoxes->AddProperties(handle);


				int32 index = 0;
				checkBoxes->CheckDefaultObject(index++, data.Montage);
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.bCanMove);
				checkBoxes->CheckDefaultValue(index++, data.Power);
				checkBoxes->CheckDefaultValue(index++, data.Launch);
				checkBoxes->CheckDefaultValue(index++, data.StopTime);
				//checkBoxes->CheckDefaultValue(index++, data.Sound);
				checkBoxes->CheckDefaultValue(index++, data.Effect);
				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
			}
		}//if(RefreshByCheckBoxes)
	}
}