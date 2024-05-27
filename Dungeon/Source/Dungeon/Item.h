#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Item.generated.h"





UCLASS()
class DUNGEON_API AItem : public AEntity {
	GENERATED_BODY()
	




	// Initialization
public:
	AItem();
protected:
	virtual void BeginPlay() override;

	// Spawn
protected:
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// Hitbox
public:
	virtual void OnInteract(AEntity* value) override;

};
