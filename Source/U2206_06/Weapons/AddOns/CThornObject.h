#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CThornObject.generated.h"

UCLASS()
class U2206_06_API ACThornObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ACThornObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
