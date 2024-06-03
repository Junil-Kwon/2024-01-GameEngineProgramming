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

	// Spawn
protected:
	virtual void OnStart  () override;
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// Sprite
protected:
	virtual void UpdateSprite(float DeltaTime) override;

	// Action
protected:
	virtual bool VerifyAction(Action value) override;
	virtual bool UpdateInputs(float DeltaTime) override;
	virtual void UpdateAction(float DeltaTime) override;
};
