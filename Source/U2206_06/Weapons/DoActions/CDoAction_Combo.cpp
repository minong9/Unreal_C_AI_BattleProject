#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"

void UCDoAction_Combo::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1);

	if(bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());

	Super::DoAction();
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;
	DoActionDatas[++Index].DoAction(OwnerCharacter);
	
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	DoActionDatas[Index].PlayEffect(OwnerCharacter->GetWorld(), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());
	Index = 0;
}

void UCDoAction_Combo::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();

	float angle = -2.0f; //�ڿ� ��ü�� ���ų� �տ� �ƹ��� ������ -2�� ����(Ÿ���� �����ϸ� -1 ~ 1�� ����)
	ACharacter* candidate = nullptr;

	for(ACharacter* hitted : Hitted)
	{
		FVector direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
		direction = direction.GetSafeNormal2D();

		FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();

		float dot = FVector::DotProduct(direction, forward);
		if(dot >= angle)
		{
			angle = dot;
			candidate = hitted;
		}
	}

	if(!!candidate)
	{
		//�¿� ������� ���鿡 ����� ��
		//�¿� ������ Ÿ������ ��(abs �ʿ�)
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());
		FRotator target = FRotator(0, rotator.Yaw, 0);

		AController* controller = OwnerCharacter->GetController<AController>();
		controller->SetControlRotation(target);
	}

	Hitted.Empty();
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
	CheckNull(InOther);

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	//Add�� AddUnique�� ������ : AddUnique�� �����̳� �ȿ� �ߺ��� ���� ������ �߰����� ����
	Hitted.AddUnique(InOther);

	CheckTrue(HitDatas.Num() < 1);
	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);
}

