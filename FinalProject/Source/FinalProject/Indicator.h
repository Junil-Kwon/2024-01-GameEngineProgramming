#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Indicator.generated.h"





UCLASS()
class FINALPROJECT_API AIndicator : public AEntity {
	GENERATED_BODY()
	




	// Initialization
public:
	AIndicator();
protected:
	virtual void BeginPlay() override;





	// Indicator
private:
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lBorderComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rBorderComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lHealthComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lHBoostComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rHealthComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lShieldComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rShieldComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lEnergeComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rEnergeComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* iShieldComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* iLeaderComponent;
	UFUNCTION() void SetupComponent(UStaticMeshComponent* component);

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY() class ACreature* creature;
	float width  = 0.0f;
	float health = 0.0f, healthMax = 0.0f;
	float shield = 0.0f, shieldMax = 0.0f;
	float energe = 0.0f, energeMax = 0.0f;
	float hboost = 0.0f;
	Group group = Group::None;
	bool  leader = false;
	void SetWidth();
	void SetGroup();
	void SetLeader();
public:
	void SetTarget(ACreature* value);
};
