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

	// Object Pool
protected:
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// Hitbox
public:
	virtual void OnInteract(AEntity* value) override;





	// Action
protected:
	virtual bool VerifyAction(Action value) override;
	virtual bool UpdateAction(float DeltaTime) override;





	// Chest
private:
	UPROPERTY(EditAnywhere) TArray<Identifier> lootArray;
public:
	void AddLoot(Identifier value, int32 num = 1);
};
