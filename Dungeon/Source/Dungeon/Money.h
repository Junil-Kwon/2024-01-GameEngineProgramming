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

	// Object Pool
protected:
	virtual void OnSpawn() override;





	// Hitbox
public:
	virtual void OnCollision(AEntity* entity) override;
	virtual void OnInteract (AEntity* entity) override;





	// Action
protected:
	virtual bool UpdateAction(float DeltaTime) override;





	// Money
	#define ValueMax 5
private:
	int32 moneyValue = 1;
	int32 mergeUpper = 1;
	bool  merged = false;
public:
	void SetValue(int32 value);
};
