#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "XBow.generated.h"





UCLASS()
class DUNGEON_API AXBow : public AWeapon {
	GENERATED_BODY()
	




	// Initialization
public:
	AXBow();

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
