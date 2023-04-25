#include "Component/CMontagesComponent.h"
#include "Global.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Character.h"

//#define LOG_UCMontagesComponent 1

UCMontagesComponent::UCMontagesComponent()
{


}

void UCMontagesComponent::PlayBackStepMode()
{
	PlayAnimMontage(EStateType::BackStep);

}

void UCMontagesComponent::PlayDeadMode()
{
	PlayAnimMontage(EStateType::Dead);

}

void UCMontagesComponent::PlayAnimMontage(EStateType InType)
{
	CheckNull(OwnerCharacter);

	FMontagesData* data = Datas[(int32)InType];

	if(data == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "Not Datas");

		return;
	}

	if (data->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "Not Montage");

		return;
	}

	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
}


void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DataTable == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "DataTable is not selected");

		return;
	}

	CheckNull(DataTable);

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<FMontagesData*> datas;
	DataTable->GetAllRows<FMontagesData>("", datas);

	for(int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for(FMontagesData* data : datas)
		{
			if((EStateType)i == data->Type)
			{
				Datas[i] = data;

				continue;
			}
		}//for(data)
	}//for(i)

#if LOG_UCMontagesComponent

	for (FMontagesData* data : datas)
	{
		if (data == nullptr)
			continue;

		FString str;
		str.Append(StaticEnum<EStateType>()->GetValueAsString(data->Type)); //Enum Ÿ���� ���ڿ��� �ٲ� ���
		str.Append(" / ");
		str.Append(data->Montage->GetPathName());

		CLog::Log(str);
	}

#endif

}




