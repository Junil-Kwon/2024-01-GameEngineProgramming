#pragma once

#include "CoreMinimal.h"
#include "Hero.h"
#include "Necromancer.generated.h"





UCLASS()
class DUNGEON_API ANecromancer : public AHero {
	GENERATED_BODY()





	// Initialization
public:
	ANecromancer();

	// Spawn
protected:
	virtual void OnStart  () override;
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// Sprite
protected:
	virtual void UpdateSprite(float DeltaTime) override;

	// Action
protected:
	virtual bool VerifyAction(Action value) override;
	virtual void UpdateAction(float DeltaTime) override;
};
