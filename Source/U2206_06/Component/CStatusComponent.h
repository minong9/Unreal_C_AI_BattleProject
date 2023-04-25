#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U2206_06_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100;

public:
	FORCEINLINE float GetHealth() { return Health; }

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:
	void Damage(float InAmount);

private:
	class ACharacter* OwnerCharacter;

private:
	float Health;
		
};
