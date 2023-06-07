#include "SWeaponDetailsView.h"
#include "SWeaponCheckBoxes.h"
#include "SEquipmentData.h"
#include "SDoActionData.h"
#include "SHitData.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailPropertyRow.h"
#include "Weapons/CWeaponAsset.h"
#include "Animation/AnimMontage.h"

#include "Particles/ParticleSystem.h"
#include "NiagaraSystem.h"
#include "Sound/SoundWave.h"

bool SWeaponDetailsView::RefreshByCheckBoxes = false;

TSharedRef<IDetailCustomization> SWeaponDetailsView::MakeInstance()
{
	return MakeShareable(new SWeaponDetailsView());
}

void SWeaponDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type = UCWeaponAsset::StaticClass();

	//DetailBuilder.HideCategory("CWeaponAsset");

	//Attachment
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("ClassSettings", FText::FromName("Class Settings"));
		category.AddProperty("AttachmentClass", type);
		category.AddProperty("EquipmentClass", type);
		category.AddProperty("DoActionClass", type);
		category.AddProperty("SubActionClass", type);

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
				checkBoxes->CheckDefaultObject(index++, data.Effect);
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
				checkBoxes->CheckDefaultObject(index++, data.Sound);
				checkBoxes->CheckDefaultObject(index++, data.Effect);
				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
			}
		}//if(RefreshByCheckBoxes)
	}
}