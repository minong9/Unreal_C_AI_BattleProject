#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.generated.h"

USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bUseControlRotation = true;
};

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bFixedCamera = true;

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

public:
	void DoAction(class ACharacter* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);
};

USTRUCT()
struct FHitData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(EditAnywhere)
		float Launch = 100;

	UPROPERTY(EditAnywhere)
		float StopTime;

	UPROPERTY(EditAnywhere)
		class USoundWave* Sound;

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

public:
	void SendDamage(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);
	void PlayMontage(class ACharacter* InOwner);
	void PlayHitStop(UWorld* InWorld);
	void PlaySoundWave(class ACharacter* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);

	void EndHitted(class ACharacter* InOwner);
};


USTRUCT()
struct FActionDamageEvent
	: public FDamageEvent
{
	GENERATED_BODY()

public:
	//포인터로 넘기는 이유는 복사방지 겸 데이터가 안넘어 올 수도 있기 때문
	FHitData* HitData;
};

UCLASS()
class U2206_06_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
	
};
