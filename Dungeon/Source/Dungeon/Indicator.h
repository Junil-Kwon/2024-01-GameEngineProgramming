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





	// Indicator
private:
	UPROPERTY() class UStaticMeshComponent* lBorderComponent;
	UPROPERTY() class UStaticMeshComponent* rBorderComponent;
	UPROPERTY() class UStaticMeshComponent* lHealthComponent;
	UPROPERTY() class UStaticMeshComponent* lHBoostComponent;
	UPROPERTY() class UStaticMeshComponent* rHealthComponent;
	UPROPERTY() class UStaticMeshComponent* lShieldComponent;
	UPROPERTY() class UStaticMeshComponent* rShieldComponent;
	UPROPERTY() class UStaticMeshComponent* lEnergeComponent;
	UPROPERTY() class UStaticMeshComponent* rEnergeComponent;
	UPROPERTY() class UStaticMeshComponent* iShieldComponent;
	UPROPERTY() class UStaticMeshComponent* iLeaderComponent;
	UFUNCTION() void SetupComponent(UStaticMeshComponent* component);
private:
	UPROPERTY() class ACreature* parent;
	float width  = 0.0f;
	float health = 0.0f, healthMax = 0.0f;
	float shield = 0.0f, shieldMax = 0.0f;
	float energe = 0.0f, energeMax = 0.0f;
	float hboost = 0.0f;
	Group group = Group::None;
	bool  leader = false;
	void SetWidth();
	void SetColor();
	void SetLeader();
private:
	bool active = false;
public:
	bool GetActive();
	void SetActive(bool value);
	virtual void Tick(float DeltaTime) override;





	// Action
public:
	virtual bool OnInteract(AEntity* entity) override;
};
