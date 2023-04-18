#include "Weapons/CAttachment.h"
#include "Global.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"

ACAttachment::ACAttachment()
{
	CHelpers::CreateComponent(this, &Root, "Root");

}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	//�浹ü ��� ��������
	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(true, children);
	for(USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);

		if (!!shape)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);

			Collisions.Add(shape);
		}
	}
	OffCollisions();

	Super::BeginPlay();
	
}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter,Cast<ACharacter>(OtherActor));
}

void ACAttachment::OnCollisions()
{
	//�̺�Ʈ�� ���� �ൿ���� �ִ°� ����
	//���� �ൿ ���Ŀ� �ʿ��ϴ�, ���ĸ� ���� ����Ѵ� �ϸ�
	//BeginOnCollisions, EndOnCollisions������ ���� ��
	//�츮�� �տ� �ִ� �ɷ� ���Ͻ�Ŵ

	if (OnAttachmentBeginCollision.IsBound())
		OnAttachmentBeginCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACAttachment::OffCollisions()
{
	if (OnAttachmentEndCollision.IsBound())
		OnAttachmentEndCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


