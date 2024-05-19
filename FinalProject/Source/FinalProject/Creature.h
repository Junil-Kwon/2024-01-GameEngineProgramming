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
	bool operator==(const ACreature& other) const;



protected:
	UPROPERTY(VisibleAnywhere) class AIndicator* indicator;
	UPROPERTY(EditAnywhere, Category = "Creature") float indicatorWidth = 24.0f;

	UPROPERTY(EditAnywhere, Category = "Creature") float healthMax = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Creature") float damage    = 0.0f;

protected:
	float health;
public:
	float GetHealth();
	void  SetHealth(float value);
	void  AdjustHealth(float value);
	virtual void Die();
};
