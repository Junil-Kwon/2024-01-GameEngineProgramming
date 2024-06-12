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

	// Spawn
protected:
	virtual void OnStart  () override;
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;

	// Update
protected:
	virtual void Update(float DeltaTime) override;





	// =========================================================================================================
	// Components
	// =========================================================================================================
	
	// Hitbox
protected:
	virtual void OnHitboxChanged() override;
	virtual void OnCollision(AEntity* entity) override;
public:
	TArray<ACreature*> melee;
	void Melee(FVector location, float range, float damage = 0.0f);
	
	// Sensor
protected:
	UPROPERTY(EditAnywhere) float defaultSensorRange;
private:
	class UCapsuleComponent* sensorComponent;
	float sensorRange;
protected:
	TArray<ACreature*> sensorArray;
	virtual void UpdateSensor(float DeltaTime);
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
	virtual void UpdateMagnet(float DeltaTime);
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
	// Properties
	// =========================================================================================================

	// Tag
public:
	virtual bool AddTag   (Tag value) override;
	virtual bool RemoveTag(Tag value) override;





	// =========================================================================================================
	// AI
	// =========================================================================================================
	
	// Target
private:
	ACreature* target;
protected:
	bool       HasTarget();
	ACreature* GetTarget();
	void       SetTarget(ACreature* value);
	typedef bool (*FTargetPredicateFunction)(ACreature*);
	virtual void SearchTarget(FTargetPredicateFunction match = nullptr);

	// Sprite
private:
	Action sprite;
	float  spriteDelay;
public:
	Action GetSprite();
	void   SetSprite(Action value);
	float  GetSpriteDelay();
	void   SetSpriteDelay(float delay);
protected:
	virtual void UpdateSprite(float DeltaTime);

	// Action
#define PlayerNearby 160.0f
private:
	Action action;
	float  actionDelay;
	float  actionCooldown[static_cast<uint8>(Action::Length)];
public:
	Action GetAction();
	bool   SetAction(Action value);
	float  GetActionDelay();
	void   SetActionDelay(float delay);
	float  GetActionCooldown(Action value);
	void   SetActionCooldown(Action value, float cooldown);
protected:
	virtual bool VerifyAction(Action value);
	virtual bool UpdateInputs(float DeltaTime);
	virtual void UpdateAction(float DeltaTime);

	// Weapon
protected:
	UPROPERTY(EditAnywhere) Identifier defaultWeapon;
private:
	class AWeapon* weapon;
public:
	bool     HasWeapon();
	AWeapon* GetWeapon();
	virtual void SetWeapon(AWeapon* value);





	// =========================================================================================================
	// Properties
	// =========================================================================================================

	// Properties
public:
	virtual void Damage(AEntity* entity, float value) override;

	// =========================================================================================================
	// Stats
	// =========================================================================================================
	
	// Stats
	#define MendCooldown 4.0f
	#define HurtCooldown 0.2f
protected:
	UPROPERTY(EditAnywhere) float defaultHealth;
	UPROPERTY(EditAnywhere) float defaultArmour;
	UPROPERTY(EditAnywhere) float defaultEnerge;
	UPROPERTY(EditAnywhere) float defaultDamage;
	UPROPERTY(EditAnywhere) float defaultAttackRange;
	UPROPERTY(EditAnywhere) float defaultAttackDelay;
private:
	float health, healthMax;
	float armour, armourMax;
	float energy, energyMax;
	float damage;
	float attackRange;
	float attackDelay;
	float hurtCooldown;
	float mendCooldown;
	
protected:
	virtual void OnDamaged(float value);
	virtual void OnArmourBroken();
	virtual void OnDie();
public:
	float GetHealthMax();
	float GetArmourMax();
	float GetEnergyMax();
	float GetHealth();
	float GetArmour();
	float GetEnergy();
	float GetDamage();
	float GetAttackRange();
	float GetAttackDelay();
	float AdjustHealth(float value);
	float AdjustArmour(float value);
	float AdjustEnergy(float value);
	float AdjustMaxHealth(float value);
	float AdjustMaxArmour(float value);
	float AdjustMaxEnergy(float value);
	float AdjustMaxDamage(float value);
};
