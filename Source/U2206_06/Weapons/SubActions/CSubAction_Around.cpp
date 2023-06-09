#include "Weapons/SubActions//CSubAction_Around.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

UCSubAction_Around::UCSubAction_Around()
{

}

void UCSubAction_Around::Pressed()
{
	Super::Pressed();

	CheckTrue(State->IsSubActionMode());
	State->OnSubActionMode();

	ActionData.DoAction(Owner);
}

void UCSubAction_Around::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();

}

void UCSubAction_Around::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

}
