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
	virtual void Tick(float DeltaTime) override;



	// Input
protected:
	bool* GetInput();
	FVector GetInputDirection();



	// Sensor
	#define DefaultSensorRange 200.0f
private:
	UPROPERTY() class USphereComponent* sensorComponent;
	UPROPERTY(EditAnywhere) float sensorRange = DefaultSensorRange;
protected:
	TArray<AEntity*> sensed;
	AEntity* selected;
	float GetSensorRange();
	void  SetSensorRange(float range = DefaultSensorRange);
	virtual void Select(AEntity* entity);
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
	#define DefaultIndicatorWidth 24.0f
private:
	UPROPERTY() class AIndicator* indicator;
	UPROPERTY(EditAnywhere) float indicatorWidth = DefaultIndicatorWidth;
public:
	AIndicator* GetIndicator();



	// Stats
	#define DefaultHealth 1.0f
	#define DefaultShield 0.0f
	#define DefaultDamage 0.0f
private:
	UPROPERTY(EditAnywhere) float healthMax = DefaultHealth;
	UPROPERTY(EditAnywhere) float shieldMax = DefaultShield;
	UPROPERTY(EditAnywhere) float damage    = DefaultDamage;
	float health = DefaultHealth;
	float shield = DefaultShield;
protected:
	float GetHealth();
	void  AdjustHealth(float value);
	virtual void OnDie();
};
