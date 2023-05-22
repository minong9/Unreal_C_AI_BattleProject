#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_CameraAnim.generated.h"

UCLASS()
class U2206_06_API UCAnimNotifyState_CameraAnim
	: public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Camera")
		class UCameraAnim* CameraAnim;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float PlayRatio = 1;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float BlendInTime = 0;	//���� ī�޶󿡼� �ش� ī�޶�� �ٲ�� �ð�

	UPROPERTY(EditAnywhere, Category = "Camera")
		float BlendOutTime = 0;

	UPROPERTY(EditAnywhere, Category = "Camera")
		TSoftObjectPtr<class ACharacter> Target;	//����������Ƽ ����, �ּ� �ε� ������ ������

	UPROPERTY(EditAnywhere, Category = "Camera")
		FVector LocationOffset = FVector(-400, 0, 90);

	UPROPERTY(EditAnywhere, Category = "Camera")
		float PitchOffset = -15;

public:
	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


private:
	class UCCameraModifier* Modifier;
};
