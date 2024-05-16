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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
	virtual void Tick(float DeltaTime) override;



	// Action
protected:
	virtual bool UpdateAction(float DeltaTime) override;

	// Group
public:
	virtual void SetGroup(Group value) override;

	// Tag
public:
	virtual bool AddTag   (Tag value) override;
	virtual bool RemoveTag(Tag value) override;

	// Effect
protected:
	virtual bool UpdateEffect(float DeltaTime) override;
public:
	virtual bool AddEffect   (Effect value, float strength, float duration) override;
	virtual bool RemoveEffect(Effect value) override;



	// Sensor
	#define DefaultSensorRange 480.0f
private:
	UPROPERTY() class USphereComponent* sensorComponent;
	UPROPERTY(EditAnywhere) float sensorRange = DefaultSensorRange;
protected:
	TArray<AEntity*> sensorArray;
	virtual bool UpdateSensor(float DeltaTime);
public:
	float GetSensorRange();
	void  SetSensorRange(float range = DefaultSensorRange);
	UFUNCTION() virtual void OnSensorBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() virtual void OnSensorEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Magent
	#define DefaultMagnetRange 120.0f
private:
	UPROPERTY() class USphereComponent* magnetComponent;
	UPROPERTY(EditAnywhere) float magnetRange = DefaultMagnetRange;
protected:
	TArray<AEntity*> magnetArray;
	virtual bool UpdateMagnet(float DeltaTime);
public:
	float GetMagnetRange();
	void  SetMagnetRange(float range = DefaultMagnetRange);
	UFUNCTION() virtual void OnMagnetBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() virtual void OnMagnetEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Select
protected:
	AEntity* selected;
	virtual void Select(AEntity* entity);



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
	#define DefaultEnerge 0.0f
	#define DefaultDamage 0.0f

	#define MendCooldown 4.0f
	#define HurtCooldown 0.2f
private:
	UPROPERTY(EditAnywhere) float health = DefaultHealth;
	UPROPERTY(EditAnywhere) float shield = DefaultShield;
	UPROPERTY(EditAnywhere) float energe = DefaultEnerge;
	UPROPERTY(EditAnywhere) float damage = DefaultDamage;
	float healthMax = DefaultHealth;
	float shieldMax = DefaultShield;
	float energeMax = DefaultEnerge;
	float hurtCooldown = 0.0f;
	float mendCooldown = 0.0f;
	
protected:
	virtual void OnDamaged(float value);
	virtual void OnShieldBroken();
	virtual void OnDie();
public:
	float GetHealth();
	void AdjustMaxHealth(float value = DefaultHealth);
	void AdjustMaxShield(float value = DefaultShield);
	void AdjustMaxEnerge(float value = DefaultEnerge);
	void AdjustMaxDamage(float value = DefaultDamage);
};
