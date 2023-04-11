#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CEquipment.generated.h"

UCLASS()
class U2206_06_API UCEquipment : public UObject
{
	GENERATED_BODY()

public:
	void BeginPlay(class ACharacter* InOwner);

private:
	class ACharacter* OwnerCharacter;
	
};
