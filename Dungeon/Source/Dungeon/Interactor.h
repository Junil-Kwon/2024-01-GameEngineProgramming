#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Interactor.generated.h"





UCLASS()
class DUNGEON_API AInteractor : public AEntity {
	GENERATED_BODY()





	// Initialization
public:
	AInteractor();
protected:
	virtual void BeginPlay() override;

	// Spawn
protected:
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;

	// Update
private:
	bool hide;
public:
	bool IsHiding();
	void Hide(bool value = true);
protected:
	virtual void Update(float DeltaTime) override;





	// Hitbox
public:
	virtual void OnInteract(AEntity* entity) override;





	// Interactor
private:
	class UTextRenderComponent* nameComponent;
	AEntity* parent;
private:
	void RefreshLocation();
};
