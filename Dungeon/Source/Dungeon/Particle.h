#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Particle.generated.h"





UCLASS()
class DUNGEON_API AParticle : public AEntity {
	GENERATED_BODY()





	// Initialization
public:
	AParticle();





	// Action
protected:
	virtual bool UpdateAction(float DeltaTime) override;
};
