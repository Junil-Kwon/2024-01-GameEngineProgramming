#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Particle.generated.h"





UCLASS()
class FINALPROJECT_API AParticle : public AEntity {
	GENERATED_BODY()
	




	// Initialization
public:
	AParticle();
protected:
	virtual	void BeginPlay() override;




	// Action
protected:
	virtual bool UpdateAction(float DeltaTime) override;
};
