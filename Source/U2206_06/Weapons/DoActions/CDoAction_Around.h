#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Around.generated.h"

UCLASS(Blueprintable)
class U2206_06_API UCDoAction_Around : public UCDoAction
{
	GENERATED_BODY()

public:
	void DoAction() override;
	void Begin_DoAction() override;
	
};
