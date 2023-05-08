#pragma once

#include "CoreMinimal.h"

class WEAPON_API SWeaponCheckBoxes
{
public:
	void AddProperties(TSharedPtr<IPropertyHandle> InHandle);

	TSharedRef<class SWidget> Draw();
	void DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, class IDetailChildrenBuilder* InChildBuilder);

private:
	TSharedRef<SWidget> DrawCheckBox(int32 InIndex);

public:
	void CheckDefaultObject(int32 InIndex, UObject* InValue); //UObject
	void CheckDefaultValue(int32 InIndex, float InValue);
	void CheckDefaultValue(int32 InIndex, bool InValue);
	void CheckDefaultValue(int32 InIndex, const FVector& InValue);

private:
	// 내부적으로만 사용하는 것엔 Internal 붙여줌
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