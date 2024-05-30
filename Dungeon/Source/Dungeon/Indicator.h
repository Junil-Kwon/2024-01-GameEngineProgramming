#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Indicator.generated.h"





UCLASS()
class DUNGEON_API AIndicator : public AEntity {
	GENERATED_BODY()
	




	// Initialization
public:
	AIndicator();
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
	virtual void Update(float DeltaTime) override;





	// Hitbox
public:
	virtual void OnInteract(AEntity* entity) override;





	// Indicator
private:
	class UStaticMeshComponent* lBorderComponent;
	class UStaticMeshComponent* rBorderComponent;
	class UStaticMeshComponent* lHealthComponent;
	class UStaticMeshComponent* lHBoostComponent;
	class UStaticMeshComponent* rHealthComponent;
	class UStaticMeshComponent* lArmourComponent;
	class UStaticMeshComponent* rArmourComponent;
	class UStaticMeshComponent* lEnergeComponent;
	class UStaticMeshComponent* rEnergeComponent;
	class UStaticMeshComponent* iArmourComponent;
	class UStaticMeshComponent* iLeaderComponent;
	void SetupComponent(UStaticMeshComponent* component);
private:
	class ACreature* parent;
	float width;
	float health, healthMax;
	float armour, armourMax;
	float energe, energeMax;
	float hboost;
	Group group;
	bool  leader;
	void SetWidth ();
	void SetColor ();
	void SetLeader();
};
