#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GreatSword.generated.h"





UCLASS()
class DUNGEON_API AGreatSword : public AWeapon {
	GENERATED_BODY()
	




	// Initialization
public:
	AGreatSword();

	// Spawn
protected:
	virtual void OnStart  () override;
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// Action
private:
	int32 pattern;
public:
	virtual bool VerifyAction(Action value) override;
protected:
	virtual void UpdateAction(float DeltaTime) override;
};
