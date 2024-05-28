#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Creature.generated.h"





UCLASS()
class DUNGEON_API ACreature : public AEntity {
	GENERATED_BODY()
	




	// =========================================================================================================
	// Setup
	// =========================================================================================================
	
	// Initialization
public:
	ACreature();
protected:
	virtual void BeginPlay() override;

	// Spawn
protected:
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// =========================================================================================================
	// Components
	// =========================================================================================================
	
	// Hitbox
protected:
	virtual void OnHitboxChanged() override;
	virtual void OnCollision(AEntity* entity) override;
	
	// Sensor
protected:
	UPROPERTY(EditAnywhere) float defaultSensorRange;
private:
	class UCapsuleComponent* sensorComponent;
	float sensorRange;
protected:
	TArray<AEntity*> sensorArray;
	virtual bool UpdateSensor(float DeltaTime);
public:
	float GetSensorRange();
	void  SetSensorRange(float value);
	UFUNCTION() void OnSensorBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnSensorEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Magnet
protected:
	UPROPERTY(EditAnywhere) float defaultMagnetRange;
private:
	class UCapsuleComponent* magnetComponent;
	float magnetRange;
protected:
	TArray<AEntity*> magnetArray;
	virtual bool UpdateMagnet(float DeltaTime);
public:
	float GetMagnetRange();
	void  SetMagnetRange(float value);
	UFUNCTION() void OnMagnetBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnMagnetEndOverlap(
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
	class AWeapon* weapon;
protected:
	bool     HasWeapon();
	AWeapon* GetWeapon();
	virtual void SetWeapon(AWeapon* value);

	// Indicator
protected:
	UPROPERTY(EditAnywhere) float defaultIndicatorWidth;
private:
	class AIndicator* indicator;
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
	// Properties
	// =========================================================================================================

	// Properties
public:
	virtual void Damage(float value) override;

	// =========================================================================================================
	// Stats
	// =========================================================================================================
	
	// Stats
	#define MendCooldown 4.0f
	#define HurtCooldown 0.2f
private:
	UPROPERTY(EditAnywhere) float defaultHealth;
	UPROPERTY(EditAnywhere) float defaultArmour;
	UPROPERTY(EditAnywhere) float defaultEnerge;
	UPROPERTY(EditAnywhere) float defaultDamage;
	float health, healthMax;
	float armour, armourMax;
	float energe, energeMax;
	float damage;
	float hurtCooldown;
	float mendCooldown;
	
protected:
	virtual void OnDamaged(float value);
	virtual void OnArmourBroken();
	virtual void OnDie();
public:
	float GetHealthMax();
	float GetArmourMax();
	float GetEnergeMax();
	float GetHealth();
	float GetArmour();
	float GetEnerge();
	float GetDamage();
	void AdjustHealth(float value);
	void AdjustArmour(float value);
	void AdjustEnerge(float value);
	void AdjustMaxHealth(float value);
	void AdjustMaxArmour(float value);
	void AdjustMaxEnerge(float value);
	void AdjustMaxDamage(float value);
};
