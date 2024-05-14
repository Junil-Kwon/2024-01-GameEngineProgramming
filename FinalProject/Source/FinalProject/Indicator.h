#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Indicator.generated.h"



UCLASS()
class FINALPROJECT_API AIndicator : public AEntity {
	GENERATED_BODY()
	
public:
	AIndicator();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;



	// Sprite
private:
	float width = 16.0f;
	float healthRatio = 1.0f;
	float shieldRatio = 1.0f;
	bool  shield = true;
	bool  leader = true;
protected:
	UPROPERTY() class USphereComponent* sphereComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lBorderComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rBorderComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lHealthComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rHealthComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lShieldComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rShieldComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* iShieldComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* iLeaderComponent;
	UPROPERTY(EditAnywhere) class UTextRenderComponent* nameComponent;
	UPROPERTY(EditAnywhere) class UTextRenderComponent* textComponent;
	void SetupComponent(UStaticMeshComponent* component);
	void SetupMaterial(UStaticMeshComponent* component);
public:
	void SetWidth(float width);
	void SetGroup(Group value);
	void SetShield(bool enable);
	void SetLeader(bool enable);
	void SetHealthRatio(float value);
	void SetShieldRatio(float value);
};
