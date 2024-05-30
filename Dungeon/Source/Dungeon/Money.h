#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Money.generated.h"





UCLASS()
class DUNGEON_API AMoney : public AEntity {
	GENERATED_BODY()





	// Initialization
public:
	AMoney();
protected:
	virtual void BeginPlay() override;

	// Spawn
protected:
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;

	// Update
public:
	virtual void Tick(float DeltaTime) override;





	// Hitbox
public:
	virtual void OnCollision(AEntity* entity) override;
	virtual void OnInteract (AEntity* entity) override;





	// Money
	#define MoneyValueMin 1
	#define MoneyValueMax 5
private:
	int32 moneyValue;
	int32 mergeUpper;
	bool  merged;
public:
	void SetValue(int32 value);
};
