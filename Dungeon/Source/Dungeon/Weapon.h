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

	// Update
public:
	virtual void Tick(float DeltaTime) override;




	// Action
private:
	UPROPERTY() class ACreature* parent;
public:
	virtual bool UpdateAction(float DeltaTime) override;
	virtual bool OnInteract(AEntity* value) override;





	// Weapon

};
