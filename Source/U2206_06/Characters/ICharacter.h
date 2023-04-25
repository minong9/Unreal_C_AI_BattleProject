#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class U2206_06_API IICharacter
{
	GENERATED_BODY()

public:
	virtual void End_BackStep() {};
	virtual void End_Hitted() {};
	virtual void End_Dead() {};


public:
	void Change_Color(class ACharacter* InCharacter, FLinearColor InColor);
};
