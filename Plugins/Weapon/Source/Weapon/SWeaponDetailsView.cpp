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

		//üũ�ڽ��� �� ������ ���� ����
		//������ üũ ������ �� ���ư��� �Ǿ� ����
		//�׷��� Refresh�� ����Ǿ��ٸ� �Ʒ� ���� ���� �ȵǰ� �ؾ���
		if(RefreshByCheckBoxes == false)
		{
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
				//�ڽ��� �ڽ�
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