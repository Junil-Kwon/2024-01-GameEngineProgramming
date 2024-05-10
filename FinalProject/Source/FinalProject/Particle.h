#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Particle.generated.h"



UENUM(BlueprintType)
enum class Identifier : uint8 {
	None			= 0,
	Dust			,
	Max				UMETA(Hidden),
};



UCLASS()
class FINALPROJECT_API AParticle : public AActor {
	GENERATED_BODY()
	
public:
	AParticle();

protected:
	virtual	void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;



protected:
	UPROPERTY(EditAnywhere) class UBoxComponent* boxComponent;
	UPROPERTY(EditAnywhere) class UStaticMeshComponent* meshComponent;

protected:
	UPROPERTY(EditAnywhere) Identifier identifier = Identifier::None;
	int32 spriteIndex = 0;
	float spriteDelay = 0.0f;
	bool  spriteXflip = false;
	void UpdateSprite(float DeltaTime);
};
