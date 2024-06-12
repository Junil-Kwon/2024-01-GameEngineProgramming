#pragma once

#include "CoreMinimal.h"
#include "Hero.h"
#include "Wizard.generated.h"





UCLASS()
class DUNGEON_API AWizard : public AHero {
	GENERATED_BODY()





	// Initialization
public:
	AWizard();

	// Spawn
protected:
	virtual void OnStart  () override;
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// Sprite
protected:
	virtual void UpdateSprite(float DeltaTime) override;

	// Action
private:
	AWizard* clone;
protected:
	virtual bool VerifyAction(Action value) override;
	virtual void UpdateAction(float DeltaTime) override;
};
