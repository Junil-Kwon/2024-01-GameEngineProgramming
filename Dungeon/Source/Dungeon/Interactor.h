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

	// Object Pool
protected:
	virtual void OnSpawn() override;





	// Hitbox
public:
	virtual void OnInteract(AEntity* entity) override;





	// Interactor
private:
	UPROPERTY(EditAnywhere) class UTextRenderComponent* nameComponent;
	UPROPERTY() class AEntity* parent;
	bool active = false;
public:
	bool GetActive();
	void SetActive(bool value);
	virtual void Tick(float DeltaTime) override;
private:
	void UpdateLocation();
};
