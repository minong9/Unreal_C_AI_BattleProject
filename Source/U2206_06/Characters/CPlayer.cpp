#include "Characters/CPlayer.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/CWeaponComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CMontagesComponent.h"
#include "Components/InputComponent.h"

ACPlayer::ACPlayer()
{
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");


	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	//애님인스턴스 애니메이션 블루프린트
	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::BackStep:BackStep(); break;
	}
}

void ACPlayer::BackStep()
{
	Movement->EnableControlRotation();

	Montages->PlayBackStepMode();
}

void ACPlayer::End_BackStep()
{
	Movement->DisableControlRotation();

	State->SetIdleMode();
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	Movement->SetSpeed(ESpeedType::Run);
	Movement->DisableControlRotation();

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMovementComponent::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", Movement, &UCMovementComponent::OnVerticalLook);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);

	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::OnAvoid);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);

	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetFistMode);
	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
	PlayerInputComponent->BindAction("Hammer", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetHammerMode);

}

void ACPlayer::OnAvoid()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Movement->CanMove());

	CheckTrue(InputComponent->GetAxisValue("MoveForward") >= 0.0f);

	State->SetBackStepMode();
}

