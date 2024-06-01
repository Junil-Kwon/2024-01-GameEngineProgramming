#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Particle.generated.h"





UCLASS()
class DUNGEON_API AParticle : public AEntity {
	GENERATED_BODY()





	// Initialization
public:
	AParticle();
protected:
	virtual void BeginPlay() override;

	// Spawn
protected:
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;

	// Update
public:
	virtual void Update(float DeltaTime) override;
};
