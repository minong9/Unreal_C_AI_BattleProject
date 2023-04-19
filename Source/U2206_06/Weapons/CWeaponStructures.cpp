#include "Weapons/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Component/CMovementComponent.h"
#include "Animation/AnimMontage.h"

void FDoActionData::DoAction(ACharacter* InOwner)
{
    //UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOwner);
    //if (!!state)
    //    state->SetActionMode();

    UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
    if (!!movement)
        bCanMove ? movement->Move() : movement->Stop();

    if (!!InOwner && !!Montage)
        InOwner->PlayAnimMontage(Montage, PlayRate);

    if (!!movement && bFixedCamera)
        movement->EnableFixedCamera();
}

/////////////////////////////////////////////////////////////////////////////

//ApplyDamage -> Take Damage(Recieve Damage) -> AnyDamage

void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
    FActionDamageEvent e;
    e.HitData = this;   //본인 것을 그대로 넘겨줌

	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayMontage(ACharacter* InOwner)
{
    UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

	if (!!movement)
        bCanMove ? movement->Move() : movement->Stop();

    if (!!Montage)
        InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FHitData::PlayHitStop(UWorld* InWorld)
{
    CheckTrue(FMath::IsNearlyZero(StopTime));

    TArray<APawn*> pawns;
    for(AActor* actor : InWorld->GetCurrentLevel()->Actors)
    {
        APawn* pawn = Cast<ACharacter>(actor);

        if(!!pawn)
        {
            pawn->CustomTimeDilation = 1e-3f;

            pawns.Add(pawn);
        }
    }

    FTimerDelegate timerDalegate;
    timerDalegate.BindLambda([=]()
    {
        for (APawn* pawn : pawns)
            pawn->CustomTimeDilation = 1;
    });

    FTimerHandle timerHandle;
    InWorld->GetTimerManager().SetTimer(timerHandle, timerDalegate, StopTime, false);
}

void FHitData::EndHitted(ACharacter* InOwner)
{
    UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

    if (!!movement)
        movement->Move();
}
