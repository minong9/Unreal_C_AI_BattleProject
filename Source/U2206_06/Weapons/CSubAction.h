#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSubAction.generated.h"

UCLASS(Abstract)
class U2206_06_API UCSubAction : public UObject
{
	GENERATED_BODY()

public:
	UCSubAction();

public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);

public:
	virtual void Pressed() {}
	virtual void Released() {}

public:
	UFUNCTION(BlueprintNativeEvent)
		void Begin_SubAction();
	virtual void Begin_SubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void End_SubAction();
	virtual void End_SubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {}



protected:
	class ACharacter* Owner;
	class ACAttachment* Attachment;
	class UCDoAction* DoAction;

	class UCStateComponent* State;
	class UCMovementComponent* Movement;
};
