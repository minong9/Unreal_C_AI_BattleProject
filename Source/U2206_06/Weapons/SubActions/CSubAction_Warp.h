#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "CSubAction_Warp.generated.h"


UCLASS()
class U2206_06_API UCSubAction_Warp : public UCSubAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		TSubclassOf<class ACameraActor> CameraActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector CameraRelativeLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		TEnumAsByte<ECameraProjectionMode::Type> ProjectionMode;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float OrthoWidth = 2000;

};
