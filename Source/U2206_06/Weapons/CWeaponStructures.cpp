#include "Weapons/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Animation/AnimMontage.h"

void FDoActionData::DoAction(ACharacter * InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	
	if (!!movement)
		bCanMove ? movement->Move() : movement->Stop();

	if (!!InOwner && !!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);

	if (!!movement && bFixedCamera)
		movement->EnableFixedCamera();
}

void FDoActionData::PlayEffect(UWorld * InWorld, const FVector & InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}

void FDoActionData::PlayEffect(UWorld * InWorld, const FVector & InLocation, const FRotator & InRotation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(InLocation);
	transform.SetScale3D(EffectScale);

	FVector location = InRotation.RotateVector(EffectLocation);
	transform.AddToTranslation(location);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}

///////////////////////////////////////////////////////////////////////////////

void FHitData::SendDamage(ACharacter * InAttacker, AActor * InAttackCauser, ACharacter * InOther)
{
	FActionDamageEvent e;
	e.HitData = this;

	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayMontage(ACharacter * InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	
	if (!!movement)
		bCanMove ? movement->Move() : movement->Stop();

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FHitData::PlayHitStop(UWorld * InWorld)
{
	CheckTrue(FMath::IsNearlyZero(StopTime));


	TArray<APawn*> pawns;
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		APawn* pawn = Cast<ACharacter>(actor);

		if (!!pawn)
		{
			pawn->CustomTimeDilation = 1e-3f;
			
			pawns.Add(pawn);
		}
	}


	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
	{
		for (APawn* pawn : pawns)
			pawn->CustomTimeDilation = 1;
	});

	FTimerHandle timerHandle;
	InWorld->GetTimerManager().SetTimer(timerHandle, timerDelegate, StopTime, false);
}

void FHitData::EndHitted(ACharacter * InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

	if (!!movement)
		movement->Move();
}

void FHitData::PlaySoundWave(ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FHitData::PlayEffect(UWorld * InWorld, const FVector & InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}

void FHitData::PlayEffect(UWorld * InWorld, const FVector & InLocation, const FRotator & InRotation)
{
	CheckNull(Effect);

	FTransform transform;

	FVector location = InLocation + InRotation.RotateVector(EffectLocation);
	transform.SetLocation(location);
	transform.SetScale3D(EffectScale);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}
