#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponStructures.h"
#include "NiagaraDataInterfaceExport.h"
#include "CThornObject.generated.h"

UCLASS()
class U2206_06_API ACThornObject
	: public AActor
	, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		FHitData HitData;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
		class UStaticMesh* NiagaraMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
		FRotator NiagaraMeshRotation;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
		class UFXSystemAsset* CollisionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
		FTransform CollisionEffectTransform;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class UNiagaraComponent* Niagara;
	
public:	
	ACThornObject();

protected:
	virtual void BeginPlay() override;


public:
	void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem);

private:
	UFUNCTION()
		void OnSystemFinished(class UNiagaraComponent* PSystem);

private:
	FVector BoxExtent;
};
