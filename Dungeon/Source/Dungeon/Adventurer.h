#pragma once

#include "CoreMinimal.h"
#include "Hero.h"
#include "Adventurer.generated.h"





UCLASS()
class DUNGEON_API AAdventurer : public AHero {
	GENERATED_BODY()
	




	// Initialization
public:
	AAdventurer();

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
