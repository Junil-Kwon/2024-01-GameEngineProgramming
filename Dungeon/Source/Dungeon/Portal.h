#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Ghost.h"
#include "Portal.generated.h"





UCLASS()
class DUNGEON_API APortal : public AEntity {
	GENERATED_BODY()
	




	// Initialization
public:
	APortal();

	// Spawn
protected:
	virtual void OnStart  () override;
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// Hitbox
protected:
	UPROPERTY(EditAnywhere) Stage defaultStage;
private:
	Stage stage;
public:
	virtual void OnCollision(AEntity* entity) override;
	virtual void OnInteract (AEntity* entity) override;
};
