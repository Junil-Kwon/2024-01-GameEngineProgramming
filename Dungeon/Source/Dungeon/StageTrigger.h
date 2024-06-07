#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "StageTrigger.generated.h"





UCLASS()
class DUNGEON_API AStageTrigger : public AEntity {
	GENERATED_BODY()
	




	// Initialization
public:
	AStageTrigger();

	// Spawn
protected:
	virtual void OnStart  () override;
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// Hitbox
public:
	virtual void OnCollision(AEntity* entity) override;
	virtual void OnInteract (AEntity* entity) override;
};
