#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "Hero.generated.h"





UCLASS()
class DUNGEON_API AHero : public ACreature {
	GENERATED_BODY()
	




	// Initialization
public:
	AHero();
protected:
	virtual void BeginPlay() override;

	// Spawn
protected:
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// Action
protected:
	virtual bool VerifyAction(Action value) override;
	virtual bool UpdateInputs(float DeltaTime) override;
	virtual bool UpdateAction(float DeltaTime) override;
};
