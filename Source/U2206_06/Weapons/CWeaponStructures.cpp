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
