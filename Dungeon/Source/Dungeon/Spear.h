#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Spear.generated.h"





UCLASS()
class DUNGEON_API ASpear : public AWeapon {
	GENERATED_BODY()
	




	// Initialization
public:
	ASpear();

	// Spawn
protected:
	virtual void OnStart  () override;
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// Update
public:
	virtual bool VerifyAction(Action value) override;
protected:
	virtual void UpdateAction(float DeltaTime) override;
};
