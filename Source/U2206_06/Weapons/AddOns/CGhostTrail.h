#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGhostTrail.generated.h"

UCLASS()
class U2206_06_API ACGhostTrail : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		float StartDelay = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		float Interval = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		FLinearColor Color = FLinearColor(1, 1, 1, 1);

	//안쪽에서부터 바깥쪽으로 퍼져가는 값(안에 옅고 바깥에 진함)
	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		float Exponent = 1;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UPoseableMeshComponent* Mesh;

	
public:	
	ACGhostTrail();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	class ACharacter* Owner;
	class UMaterialInstanceDynamic* Material;

	FTimerHandle TimerHandle;
};
