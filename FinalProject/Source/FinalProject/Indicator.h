#pragma once

#include "Entity.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Indicator.generated.h"



UCLASS()
class FINALPROJECT_API AIndicator : public AActor {
	GENERATED_BODY()
	
public:
	AIndicator();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;



	// Identifier
private:
	UPROPERTY(VisibleAnywhere) Identifier identifier = Identifier::Indicator;

	// Sprite
private:
	float width = 16.0f;
	float healthRatio = 1.0f;
	float shieldRatio = 1.0f;
	bool  shield = true;
	bool  leader = true;
protected:
	UPROPERTY() class UStaticMesh* sprite;
	UPROPERTY(EditAnywhere) class USphereComponent* sphereComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lBorderComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rBorderComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lHealthComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rHealthComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* lShieldComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* rShieldComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* iShieldComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* iLeaderComponent;
	//UPROPERTY(EditAnywhere) class UStaticMeshComponent* text;
	UFUNCTION() void SetSpriteDefault(UStaticMeshComponent* component);
protected:
	void SetWidth(float value);
	void SetGroup(Group value);
	void SetShield(bool enable, float value = 1.0f);
	void SetLeader(bool enable);
	void SetHealthRatio(float value);
	void SetShieldRatio(float value);
};
