#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Components/TimelineComponent.h"
#include "CSubAction_Aiming.generated.h"

USTRUCT()
struct FAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TargetArmLength = 100;

	UPROPERTY(EditAnywhere)
		FVector SocketOffset = FVector(0, 30, 10);

	UPROPERTY(EditAnywhere)
		bool bEnableCameraLag;

	UPROPERTY(EditAnywhere)
		FVector CameraLocation;
};

UCLASS(Blueprintable)
class U2206_06_API UCSubAction_Aiming : public UCSubAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Aiming")
		class UCurveFloat* Curve;

	UPROPERTY(EditAnywhere, Category = "Aiming")
		FAimData AimData;

	UPROPERTY(EditAnywhere, Category = "Aiming")
		float AimingSpeed = 200;

public:
	UCSubAction_Aiming();

public:
	virtual void Pressed() override;;
	virtual void Released() override;

public:
	void BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction) override;

public:
	void Tick_Implementation(float InDeltaTime) override;

private:
	UFUNCTION()
		void OnAiming(float Output);

private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

private:
	bool bAiming;

private:
	FAimData OriginData;

private:
	FTimeline Timeline;
};
