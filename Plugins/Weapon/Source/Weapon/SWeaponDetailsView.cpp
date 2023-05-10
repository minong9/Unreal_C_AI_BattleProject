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
//		//Editcategory : 있으면 리턴해주고, 없으면 추가해줌
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
//		//수정할 데이터 항목(헤더, 콤보박스ㅁ)을 클릭하면 해당 상세데이터(자식)가 보이게 하기
//		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type);
//
//		//체크박스를 다 날리고 새로 만듬
//		//기존의 체크 정보는 다 날아가게 되어 있음
//		//그래서 Refresh로 실행되었다면 아래 줄은 콜이 안되게 해야함
//		if(RefreshByCheckBoxes == false)
//		{
//			TSharedPtr<SWeaponCheckBoxes> checkBoxes = SEquipmentData::CreateCheckBoxes();
//			checkBoxes->AddProperties(row.GetPropertyHandle());
//
//			//기본값 선언
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
//			//자식의 자식
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
//				//기본값
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
//			//자식의 자식
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
//				//기본값
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