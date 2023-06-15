#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Bow.generated.h"

UCLASS()
class U2206_06_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class UPoseableMeshComponent* Mesh;

public:
	ACAttachment_Bow();

protected:
	virtual void BeginPlay() override;

	void OnBeginEquip_Implementation() override;
	void OnUnequip_Implementation() override;
};
