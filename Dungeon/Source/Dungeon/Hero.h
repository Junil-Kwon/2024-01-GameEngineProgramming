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





	// Hitbox
public:
	virtual void OnInteract(AEntity* value) override;
};
