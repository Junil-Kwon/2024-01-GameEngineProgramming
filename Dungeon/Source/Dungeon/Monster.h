#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "Monster.generated.h"





UCLASS()
class DUNGEON_API AMonster : public ACreature {
	GENERATED_BODY()
		




	// Initialization
public:
	AMonster();

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
