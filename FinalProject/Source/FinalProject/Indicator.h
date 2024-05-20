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
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rHealthComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lShieldComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rShieldComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lEnergeComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rEnergeComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* hpBoostComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* iShieldComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* iLeaderComponent;
	UFUNCTION() void SetupComponent(UStaticMeshComponent* component);
	float width = 24.0f;
	float healthRatio = 1.0f;
	float shieldRatio = 0.0f;
	float energeRatio = 0.0f;
	bool  leader = false;
public:
	virtual void Tick(float DeltaTime) override;

	void SetWidth(float value);
	void SetGroup(Group value);
	void SetHealthRatio(float value = -1);
	void SetShieldRatio(float value = -1);
	void SetEnergeRatio(float value = -1);
	void SetLeader(bool value);
};
