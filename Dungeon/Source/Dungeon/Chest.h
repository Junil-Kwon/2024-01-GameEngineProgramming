#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Chest.generated.h"





UCLASS()
class DUNGEON_API AChest : public AEntity {
	GENERATED_BODY()
	




	// Initialization
public:
	AChest();

	// Spawn
protected:
	virtual void OnStart  () override;
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;

	// Update
private:
	Action action;
	float  actionDelay;
public:
	virtual void Update(float DeltaTime) override;





	// Hitbox
public:
	virtual void OnInteract(AEntity* value) override;





	// Chest
private:
	UPROPERTY(EditAnywhere) TArray<Identifier> lootArray;
	int32 loot;
public:
	void AddLoot(Identifier value, int32 num = 1);
};
