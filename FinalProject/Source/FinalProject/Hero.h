#pragma once

#include "CoreMinimal.h"
#include "Creature.h"
#include "Hero.generated.h"



UCLASS()
class FINALPROJECT_API AHero : public ACreature {
	GENERATED_BODY()
	
public:
	AHero();
protected:
	virtual void BeginPlay() override;

	// Action
protected:
	virtual bool VerifyAction(Action value)    override;
	virtual bool UpdateInputs(float DeltaTime) override;
	virtual bool UpdateAction(float DeltaTime) override;

	// Sensor
public:
	virtual void OnSensorBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSensorEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// Stats
protected:
	virtual void OnDie() override;
};
