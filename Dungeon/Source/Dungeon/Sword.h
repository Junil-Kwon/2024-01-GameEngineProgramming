#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Sword.generated.h"





UCLASS()
class DUNGEON_API ASword : public AWeapon {
	GENERATED_BODY()
	




	// Initialization
public:
	ASword();
protected:
	virtual void BeginPlay() override;

	// Spawn
protected:
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
