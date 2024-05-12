#pragma once

#include "Entity.h"
#include "CoreMinimal.h"
#include "Entity.h"
#include "Particle.generated.h"



UCLASS()
class FINALPROJECT_API AParticle : public AActor {
	GENERATED_BODY()
	
public:
	AParticle();
protected:
	virtual	void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;



	// Identifier
private:
	UPROPERTY(EditAnywhere) Identifier identifier = Identifier::None;

	// Hitbox
private:
	UPROPERTY(EditAnywhere) class UCapsuleComponent* hitboxComponent;
	float hitboxRadius = 0.0f;
	float hitboxHeight = 0.0f;
	void  OnHitboxChanged();
public:
	float GetHitboxRadius();
	float GetHitboxHeight();
	bool  SetHitboxRadius(float radius);
	bool  SetHitboxHeight(float height);
	bool  SetHitbox(float radius, float height);
	FVector GetFootLocation();

	// Sprite
private:
	UPROPERTY() class UStaticMesh* sprite;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* spriteComponent;
	int32 spriteIndex = 0;
	float spriteScale = 1.0f;
	bool  spriteXflip = false;
protected:
	float spriteDelay = 0.0f;
	int32 GetSpriteIndex();
	float GetSpriteScale();
	bool  GetSpriteXFlip();
	bool  SetSpriteIndex(int32 value);
	bool  SetSpriteScale(float value);
	bool  SetSpriteXFlip(bool  value);
	virtual void  UpdateSprite(float DeltaTime);
};
