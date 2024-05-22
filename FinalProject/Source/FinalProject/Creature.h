#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Creature.generated.h"





UCLASS()
class FINALPROJECT_API ACreature : public AEntity {
	GENERATED_BODY()
	




	// =========================================================================================================
	// Default Functions
	// =========================================================================================================
	// Initialization
public:
	ACreature();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Update
public:
	virtual void Tick(float DeltaTime) override;





	// =========================================================================================================
	// Components
	// =========================================================================================================
	// Hitbox
protected:
	virtual void OnHitboxChanged() override;
	
	// Sensor
protected:
	UPROPERTY(EditAnywhere) float defaultSensorRange = 480.0f;
private:
	UPROPERTY() class UCapsuleComponent* sensorComponent;
	float sensorRange;
protected:
	TArray<AEntity*> sensorArray;
	virtual bool UpdateSensor(float DeltaTime);
public:
	float GetSensorRange();
	void  SetSensorRange(float value);
	UFUNCTION() virtual void OnSensorBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() virtual void OnSensorEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Magnet
protected:
	UPROPERTY(EditAnywhere) float defaultMagnetRange = 120.0f;
private:
	UPROPERTY() class UCapsuleComponent* magnetComponent;
	float magnetRange;
protected:
	TArray<AEntity*> interactability, collectable;
	virtual bool UpdateMagnet(float DeltaTime);
public:
	float GetMagnetRange();
	void  SetMagnetRange(float value);
	UFUNCTION() virtual void OnMagnetBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() virtual void OnMagnetEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Select
private:
	AEntity* selected;
protected:
	bool     HasSelected();
	AEntity* GetSelected();
	virtual void SetSelected(AEntity* entity);
	
	// Weapon
private:
	UPROPERTY() class AWeapon* weapon;
protected:
	bool     HasWeapon();
	AWeapon* GetWeapon();
	virtual void SetWeapon(AWeapon* value);

	// Indicator
protected:
	UPROPERTY(EditAnywhere) float defaultIndicatorWidth = 24.0f;
private:
	UPROPERTY() class AIndicator* indicator;
	float indicatorWidth;
public:
	AIndicator* GetIndicator();
	float GetIndicatorWidth();
protected:
	void  SetIndicatorWidth(float value);





	// =========================================================================================================
	// AI
	// =========================================================================================================
	// Action
protected:
	virtual bool UpdateAction(float DeltaTime) override;





	// =========================================================================================================
	// Stats
	// =========================================================================================================
	// Stats
	#define MendCooldown 4.0f
	#define HurtCooldown 0.2f
private:
	UPROPERTY(EditAnywhere) float health = 1.0f;
	UPROPERTY(EditAnywhere) float shield = 0.0f;
	UPROPERTY(EditAnywhere) float energe = 0.0f;
	UPROPERTY(EditAnywhere) float damage = 0.0f;
	float healthMax;
	float shieldMax;
	float energeMax;
	float hurtCooldown = 0.0f;
	float mendCooldown = 0.0f;
	
protected:
	virtual void OnDamaged(float value);
	virtual void OnShieldBroken();
	virtual void OnDie();
public:
	float GetHealth();
	float GetShield();
	float GetEnerge();
	float GetHealthMax();
	float GetShieldMax();
	float GetEnergeMax();
	void AdjustMaxHealth(float value);
	void AdjustMaxShield(float value);
	void AdjustMaxEnerge(float value);
	void AdjustMaxDamage(float value);
};
