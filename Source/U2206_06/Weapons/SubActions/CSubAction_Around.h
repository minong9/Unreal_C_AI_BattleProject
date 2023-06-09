#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CSubAction_Around.generated.h"

UCLASS()
class U2206_06_API UCSubAction_Around : public UCSubAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;

public:
	UCSubAction_Around();

public:
	virtual void Pressed() override;

	virtual void Begin_SubAction_Implementation() override;
	virtual void End_SubAction_Implementation() override;

	
};
