#pragma once

#include "CoreMinimal.h"

class WEAPON_API SWeaponCheckBoxes
	: public TSharedFromThis<SWeaponCheckBoxes>
{
public:
	void AddProperties(TSharedPtr<IPropertyHandle> InHandle);
	
	void SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities);

	TSharedRef<class SWidget> Draw(bool bBackground = false);
	void DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, class IDetailChildrenBuilder* InChildBuilder);

private:
	TSharedRef<SWidget> DrawCheckBox(int32 InIndex);

public:
	void CheckDefaultObject(int32 InIndex, UObject* InValue); //UObject
	void CheckDefaultValue(int32 InIndex, float InValue);
	void CheckDefaultValue(int32 InIndex, bool InValue);
	void CheckDefaultValue(int32 InIndex, const FVector& InValue);

private:
	void OnCheckStateChanged(ECheckBoxState InState, int32 InIndex);

private:
	TSharedPtr<class IPropertyUtilities> Utilities;

private:
	struct FInternalData
	{
		bool bChecked;
		FString Name;
		TSharedPtr<IPropertyHandle> Handle;

		FInternalData(TSharedPtr<IPropertyHandle> InHandle)
		{
			bChecked = false;
			Handle = InHandle;

			Name = Handle->GetPropertyDisplayName().ToString();
		}
	};
	TArray<FInternalData> InternalDatas;
};
