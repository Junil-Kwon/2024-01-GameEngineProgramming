#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "StalKnight.generated.h"




UCLASS()
class DUNGEON_API AStalKnight : public ACreature {
	GENERATED_BODY()





	// Initialization
public:
	AStalKnight();

	// Spawn
protected:
	virtual void OnStart  () override;
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// =========================================================================================================
	// Components
	// =========================================================================================================

	// Hitbox
protected:
	class ACreature* parent;
public:
	virtual void OnInteract(AEntity* value) override;



	// Sprite
protected:
	virtual void UpdateSprite(float DeltaTime) override;

	// Action
protected:
	virtual bool VerifyAction(Action value) override;
	virtual bool UpdateInputs(float DeltaTime) override;
	virtual void UpdateAction(float DeltaTime) override;
};
