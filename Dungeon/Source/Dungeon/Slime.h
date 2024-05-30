#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "Slime.generated.h"





UCLASS()
class DUNGEON_API ASlime : public ACreature {
	GENERATED_BODY()
		




	// Initialization
public:
	ASlime();
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
	virtual void UpdateAction(float DeltaTime) override;
};
