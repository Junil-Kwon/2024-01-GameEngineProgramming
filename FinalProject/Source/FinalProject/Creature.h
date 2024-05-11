#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Creature.generated.h"






UCLASS()
class FINALPROJECT_API ACreature : public AEntity {
	GENERATED_BODY()
	
public:
	ACreature();
protected:
	virtual void BeginPlay() override;

public:
	bool operator==(const ACreature& other) const;
	virtual void Tick(float DeltaTime) override;



	// Sensor
private:
	UPROPERTY(EditAnywhere) class USphereComponent* sensorComponent;
	float sensorRange;
protected:
	TArray<ACreature*> sensed;
	ACreature* selected;
	float GetSensorRange();
	void  SetSensorRange(float value);
	virtual void Select(ACreature* creature);
	virtual void UpdateSensor(float DeltaTime);
public:
	UFUNCTION() virtual void OnBeginSensed(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() virtual void OnEndSensed(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	
	// Indicator
private:
	UPROPERTY() class AIndicator* indicator;
	UPROPERTY(EditAnywhere) float indicatorWidth = 24.0f;
public:
	AIndicator* GetIndicator();
	

	
	// Stats
	UPROPERTY(EditAnywhere) float healthMax = 0.0f;
	UPROPERTY(EditAnywhere) float damage = 0.0f;

protected:
	float health;
public:
	float GetHealth();
	void  SetHealth(float value);
	void  AdjustHealth(float value);
	virtual void Die();
};
