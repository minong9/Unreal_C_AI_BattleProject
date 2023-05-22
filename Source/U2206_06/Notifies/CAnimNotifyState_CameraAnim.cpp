#include "Notifies/CAnimNotifyState_CameraAnim.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraAnim.h"
#include "Weapons/AddOns/CCameraModifier.h"

FString UCAnimNotifyState_CameraAnim::GetNotifyName_Implementation() const
{
	return "CameraAnim";
}

void UCAnimNotifyState_CameraAnim::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	CheckNull(CameraAnim);

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	ACharacter* target = character;
	if (Target.IsNull() == false)		//타겟이 따로 없으면 몽타주 재생하는 타겟 기준이 됨
		target = Target.Get();			//타겟이 설정되었다면 타겟으로

	APlayerController* controller = character->GetController<APlayerController>();
	CheckNull(controller);	//플레이어가 아니라는 뜻

	UCameraModifier* modifier = controller->PlayerCameraManager->AddNewCameraModifier(UCCameraModifier::StaticClass());
	Modifier = Cast<UCCameraModifier>(modifier);

	FVector location = target->GetActorLocation();

	float yaw = controller->GetControlRotation().Yaw;
	location += FRotator(0, yaw, 0).RotateVector(LocationOffset);

	Modifier->SetLocation(location);
	Modifier->SetRotation(FRotator(PitchOffset, yaw, 0));

	//카메라 정렬
	controller->SetControlRotation(FRotator(0, controller->GetControlRotation().Yaw, 0));
	controller->PlayerCameraManager->PlayCameraAnim(CameraAnim, PlayRatio, 1, BlendInTime, BlendOutTime, false, false, 0, ECameraShakePlaySpace::UserDefined, controller->GetControlRotation());
}

void UCAnimNotifyState_CameraAnim::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	CheckNull(CameraAnim);

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	APlayerController* controller = character->GetController<APlayerController>();
	CheckNull(controller);

	controller->PlayerCameraManager->RemoveCameraModifier(Modifier);
}
