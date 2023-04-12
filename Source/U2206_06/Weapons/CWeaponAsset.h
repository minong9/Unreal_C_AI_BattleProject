#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWeaponStructures.h"
#include "CWeaponAsset.generated.h"

UCLASS()
class U2206_06_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
		FEquipmentData EquipmentData;

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);

private:
	UPROPERTY()
		class ACAttachment* Attachment;	//����ȭ�� �ؼ� �������÷��Ϳ� ���� �ڵ����� �������� �ʵ�����

	UPROPERTY()
		class UCEquipment* Equipment;

};
