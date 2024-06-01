#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Projectile.generated.h"





UCLASS()
class DUNGEON_API AProjectile : public AEntity {
	GENERATED_BODY()
	




	// Initialization
public:
	AProjectile();
protected:
	virtual void BeginPlay() override;

	// Spawn
protected:
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;

	// Update
public:
	virtual void Update(float DeltaTime) override;





	// Hitbox
private:
	AEntity* parent;
public:
	virtual void OnCollision(AEntity* entity) override;
};
