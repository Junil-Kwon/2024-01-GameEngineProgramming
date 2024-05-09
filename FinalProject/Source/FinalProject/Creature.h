#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Creature.generated.h"



UCLASS()
class FINALPROJECT_API ACreature : public AEntity {
	GENERATED_BODY()
	
public:
	ACreature();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, Category = "Creature") float healthMax = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Creature") float damage = 0.0f;
	UPROPERTY() TSubclassOf<class AIndicator> indicator;

	float GetHealth();
	void  SetHealth(float value);
	void  AdjustHealth(float value);

protected:
	virtual void Die();

private:
	TArray<float> effectStrength;
	TArray<float> effectDuration;

	float health;
};
