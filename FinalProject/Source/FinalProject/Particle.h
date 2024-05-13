#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Particle.generated.h"



UCLASS()
class FINALPROJECT_API AParticle : public AEntity {
	GENERATED_BODY()

public:
	AParticle();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;



	// Sprite
protected:
	virtual void UpdateSprite(float DeltaTime) override;
};
