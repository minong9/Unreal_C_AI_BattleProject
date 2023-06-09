#include "Component/CWeaponComponent.h"
#include "Global.h"
#include "CStateComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CEquipment.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

bool UCWeaponComponent::IsIdleMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!DataAssets[(int32)Type], nullptr);

	return DataAssets[(int32)Type]->GetAttachment();
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	//현재 무기가 선택되어 있는지 체크하고 해제해주기위함
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!DataAssets[(int32)Type], nullptr);

	return DataAssets[(int32)Type]->GetEquipment();
}

UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!DataAssets[(int32)Type], nullptr);

	return DataAssets[(int32)Type]->GetDoAction();
}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	for(int32 i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(OwnerCharacter);
	}
}


void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCWeaponComponent::SetUnarmedMode()
{
	
	GetEquipment()->Unequip();

	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetFistMode()
{
	CheckFalse(IsIdleMode());


	SetMode(EWeaponType::Fist);
}

void UCWeaponComponent::SetSwordMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetHammerMode()
{
	CheckFalse(IsIdleMode());


	SetMode(EWeaponType::Hammer);
}

void UCWeaponComponent::SetWarpMode()
{
	CheckFalse(IsIdleMode());


	SetMode(EWeaponType::Warp);
}

void UCWeaponComponent::SetAroundMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Around);
}

void UCWeaponComponent::SetBowMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Bow);
}

void UCWeaponComponent::DoAction()
{
	if (!!GetDoAction())
		GetDoAction()->DoAction();
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();

		return;
	}
	else if (IsUnarmedMode() == false)
	{
		GetEquipment()->Unequip();
	}

	if (!!DataAssets[(int32)InType])
	{
		DataAssets[(int32)InType]->GetEquipment()->Equip();

		ChangeType(InType);
	}
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = Type;
	Type = InType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(prevType, InType);
}

