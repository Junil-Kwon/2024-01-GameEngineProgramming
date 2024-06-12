#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Tomahawk.generated.h"





UCLASS()
class DUNGEON_API ATomahawk : public AWeapon {
	GENERATED_BODY()
	




	// Initialization
public:
	ATomahawk();

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
