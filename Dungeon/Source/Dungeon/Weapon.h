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

	// Object Pool
protected:
	virtual void OnDespawn() override;





	// Hitbox
public:
	virtual void OnInteract(AEntity* value) override;





	// Action
private:
	UPROPERTY() class ACreature* parent;
public:
	virtual bool UpdateAction(float DeltaTime) override;





	// Weapon

};
