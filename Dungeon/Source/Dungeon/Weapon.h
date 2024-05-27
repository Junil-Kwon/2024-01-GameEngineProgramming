#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Weapon.generated.h"





UCLASS()
class DUNGEON_API AWeapon : public AEntity {
	GENERATED_BODY()





	// Initialization
public:
	AWeapon();
protected:
	virtual void BeginPlay() override;

	// Spawn
protected:
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// Hitbox
protected:
	class ACreature* parent;
public:
	virtual void OnInteract(AEntity* value) override;





	// Action
public:
	virtual bool VerifyAction(Action value) override;
};
