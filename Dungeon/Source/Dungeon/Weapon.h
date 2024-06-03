#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Weapon.generated.h"





UCLASS()
class DUNGEON_API AWeapon : public AEntity {
	GENERATED_BODY()





	// =========================================================================================================
	// Setup
	// =========================================================================================================

	// Initialization
public:
	AWeapon();

	// Spawn
protected:
	virtual void OnStart  () override;
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;

	// Update
protected:
	virtual void Update(float DeltaTime) override;





	// =========================================================================================================
	// Components
	// =========================================================================================================

	// Hitbox
protected:
	class ACreature* parent;
public:
	virtual void OnInteract(AEntity* value) override;






	// =========================================================================================================
	// AI
	// =========================================================================================================

	// Action
private:
	Action action;
	float  actionDelay;
public:
	Action GetAction();
	bool   SetAction(Action value);
	float  GetActionDelay();
	void   SetActionDelay(float value);
	virtual bool VerifyAction(Action value);
protected:
	virtual void UpdateAction(float DeltaTime);

	// Weapon
protected:
	UPROPERTY(EditAnywhere) float defaultWeaponRange;
private:
	float  weaponRange;
public:
	float  GetWeaponRange();
};
