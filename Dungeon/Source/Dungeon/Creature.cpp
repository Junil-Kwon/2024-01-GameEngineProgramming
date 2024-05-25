#include "Creature.h"
#include "Ghost.h"
#include "Weapon.h"
#include "Interactor.h"
#include "Indicator.h"

#include "Components/CapsuleComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

ACreature::ACreature() {
	defaultHitboxRadius = 36.0f;
	defaultHitboxHeight = 96.0f;
	defaultHandLocation = FVector2D(24.0f, -4.0f);
	
	sensorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sensor"));
	sensorComponent->InitCapsuleSize(0.5f, 0.5f);
	sensorComponent->SetCollisionProfileName(TEXT("Sensor"));
	sensorComponent->SetupAttachment(RootComponent);

	magnetComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Magnet"));
	magnetComponent->InitCapsuleSize(0.5f, 0.5f);
	magnetComponent->SetCollisionProfileName(TEXT("Sensor"));
	magnetComponent->SetupAttachment(RootComponent);
}
void ACreature::BeginPlay() {
	Super::BeginPlay();

	sensorComponent->OnComponentBeginOverlap.AddDynamic(this, &ACreature::OnSensorBeginOverlap);
	sensorComponent->OnComponentEndOverlap  .AddDynamic(this, &ACreature::OnSensorEndOverlap  );
	magnetComponent->OnComponentBeginOverlap.AddDynamic(this, &ACreature::OnMagnetBeginOverlap);
	magnetComponent->OnComponentEndOverlap  .AddDynamic(this, &ACreature::OnMagnetEndOverlap  );
}

// =============================================================================================================
// Object Pool
// =============================================================================================================

void ACreature::OnSpawn() {
	Super::OnSpawn();

	SetSensorRange(defaultSensorRange);
	SetMagnetRange(defaultMagnetRange);

	healthMax = health;
	shieldMax = shield;
	energeMax = energe;

	SetIndicatorWidth(defaultIndicatorWidth);
	indicator = static_cast<AIndicator*>(Spawn(Identifier::Indicator));
	indicator->SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, GetHitboxHeight() * 0.5f + 96.0f));
	indicator->OnInteract(this);
	indicator->Attach(this);
}
void ACreature::OnDespawn() {
	Super::OnDespawn();

	if (HasWeapon()) weapon->Despawn();
	indicator->Despawn();
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void ACreature::OnHitboxChanged() {
	Super::OnHitboxChanged();
	sensorComponent->SetCapsuleHalfHeight(sensorRange + GetHitboxHeight() * 0.5f - GetHitboxRadius());
	magnetComponent->SetCapsuleHalfHeight(magnetRange + GetHitboxHeight() * 0.5f - GetHitboxRadius());
}
void ACreature::OnCollision(AEntity* entity) {
	Super::OnCollision(entity);
	if (GetGhost()->GetPlayer() == nullptr) return;
	if (GetGroup() != GetGhost()->GetPlayer()->GetGroup()) return;
	if (entity->HasTag(Tag::Collectable)) entity->OnInteract(this);
}

// =============================================================================================================
// Sensor
// =============================================================================================================

bool ACreature::UpdateSensor(float DeltaTime) {
	if (sensorArray.Num() == 0) return false;
	return true;
}

float ACreature::GetSensorRange() { return sensorRange; }
void  ACreature::SetSensorRange(float value) {
	sensorRange = value;
	sensorComponent->SetCapsuleRadius(sensorRange);
}
void ACreature::OnSensorBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {
	AEntity* entity = static_cast<AEntity*>(OtherActor);
	if (OtherActor->IsA(AEntity::StaticClass()) && !sensorArray.Contains(entity)) sensorArray.Add(entity);
}
void ACreature::OnSensorEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	AEntity* entity = static_cast<AEntity*>(OtherActor);
	if (OtherActor->IsA(AEntity::StaticClass()) && sensorArray.Contains(entity)) sensorArray.Remove(entity);
}

// =============================================================================================================
// Magnet
// =============================================================================================================

bool ACreature::UpdateMagnet(float DeltaTime) {
	if (magnetArray.Num() == 0) return false;
	if (GetGhost()->GetPlayer() == nullptr) return false;
	if (GetGroup() != GetGhost()->GetPlayer()->GetGroup()) return false;

	AEntity* entity = nullptr;
	float nearest = magnetRange;
	for (int32 i = magnetArray.Num() - 1; -1 < i; i--) {
		if (magnetArray[i] == nullptr) {
			magnetArray.RemoveAt(i);
			continue;
		}
		if (magnetArray[i]->HasTag(Tag::Collectable)) {
			magnetArray[i]->AddMovementInput(GetActorLocation() - magnetArray[i]->GetActorLocation(), 1.0f);
		}
		float distance = FVector::Distance(magnetArray[i]->GetActorLocation(), GetActorLocation());
		if (magnetArray[i]->HasTag(Tag::Interactability) && distance < nearest) {
			entity = magnetArray[i];
			nearest = distance;
		}
	}
	if (HasTag(Tag::Player)) SetSelected(entity);
	return true;
}

float ACreature::GetMagnetRange() { return magnetRange; }
void  ACreature::SetMagnetRange(float value) {
	magnetRange = value;
	magnetComponent->SetCapsuleRadius(magnetRange);
}
void ACreature::OnMagnetBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {
	AEntity* entity = static_cast<AEntity*>(OtherActor);
	if (OtherActor->IsA(AEntity::StaticClass()) && !magnetArray.Contains(entity)) magnetArray.Add(entity);
}
void ACreature::OnMagnetEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	AEntity* entity = static_cast<AEntity*>(OtherActor);
	if (OtherActor->IsA(AEntity::StaticClass()) && magnetArray.Contains(entity)) {
		magnetArray.Remove(entity);
		if (GetSelected() == entity) SetSelected(nullptr);
	}
}

// =============================================================================================================
// Select
// =============================================================================================================

bool     ACreature::HasSelected() { return selected != nullptr; };
AEntity* ACreature::GetSelected() { return selected; }
void ACreature::SetSelected(AEntity* entity) {
	if (selected == entity) return;
	if (selected != nullptr) {
		if (selected->HasTag(Tag::Interactability)) selected->GetInteractor()->SetActive(false);
	}
	if (entity != nullptr) {
		if (entity->HasTag(Tag::Interactability)) entity->GetInteractor()->SetActive(true);
	}
	selected = entity;
}

// =============================================================================================================
// Weapon
// =============================================================================================================

bool     ACreature::HasWeapon() { return weapon != nullptr; }
AWeapon* ACreature::GetWeapon() { return weapon; }
void ACreature::SetWeapon(AWeapon* value) {
	if (weapon == value) return;
	if (weapon != nullptr) {
		weapon->SetActorLocation(GetActorLocation() + GetLookDirection() * GetHitboxRadius());
		weapon->LaunchCharacter(GetLookDirection() * 200.0f + FVector(0.0f, 0.0f, 200.0f), true, true);
		weapon->OnInteract(nullptr);
		weapon->Detach();
	}
	if (value != nullptr) {
		value->SetActorLocation(GetHandLocation());
		value->OnInteract(this);
		value->Attach(this);
	}
	weapon = value;
}

// =============================================================================================================
// Indicator
// =============================================================================================================

AIndicator* ACreature::GetIndicator() { return indicator; }
float ACreature::GetIndicatorWidth() { return indicatorWidth; }
void  ACreature::SetIndicatorWidth(float value) { indicatorWidth = value; }





// =============================================================================================================
// Action
// =============================================================================================================

bool ACreature::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;
	UpdateSensor(DeltaTime);
	UpdateMagnet(DeltaTime);

	switch (GetAction()) {
	case Action::Attack:
		if (HasSelected()) {
			if (GetSelected()->IsA(AWeapon::StaticClass())) SetWeapon(static_cast<AWeapon*>(GetSelected()));
			else GetSelected()->OnInteract(this);
			SetAction(Action::Idle);
			SetActionCooldown(Action::Attack, 0.25f);
			return false;
		}
		break;
	case Action::Defend:

		break;
	}
	return true;
}






// =============================================================================================================
// Stats
// =============================================================================================================

void  ACreature::OnDamaged(float value) {
	if (0.0f < value) {
		if (HasTag(Tag::Invulnerability)) return;
		if (HasEffect(Effect::Resistance)) value *= 1 - GetEffectStrength(Effect::Resistance);
		if (HasEffect(Effect::HealthBoost)) {
			float strength = GetEffectStrength(Effect::HealthBoost);
			float absorption = FMath::Max(strength, value) - FMath::Min(strength, value);
			AdjustEffectStrength(Effect::HealthBoost, -absorption);
			value -= absorption;
		}
		hurtCooldown = HurtCooldown;
		mendCooldown = MendCooldown;
	}
	float shieldTemp = shield;
	if (shield) shield = FMath::Clamp(shield - value, 0.0f, shieldMax);
	else        health = FMath::Clamp(health - value, 0.0f, healthMax);
	if (shield == 0.0f && shieldTemp) OnShieldBroken();
	if (health == 0.0f) OnDie();
}
void  ACreature::OnShieldBroken() {
	shieldMax = 0.0f;
}
void  ACreature::OnDie() {
	SetAction(Action::Defeat);

	if (HasWeapon()) SetWeapon(nullptr);
	// if (HasEffect(Effect::Burn)) smoke effect, if enough strength, ashed effect
}

float ACreature::GetHealth() { return health; }
float ACreature::GetShield() { return shield; }
float ACreature::GetEnerge() { return energe; }
float ACreature::GetHealthMax() { return healthMax; }
float ACreature::GetShieldMax() { return shieldMax; }
float ACreature::GetEnergeMax() { return energeMax; }

void ACreature::AdjustMaxHealth(float value) {
	healthMax += FMath::Min(healthMax + value, 0.0f);
	health    += FMath::Min(health    + value, 0.0f);
	if (health == 0.0f) OnDie();
}
void ACreature::AdjustMaxShield(float value) {
	shieldMax += FMath::Min(shieldMax + value, 0.0f);
	shield    += FMath::Min(shield    + value, 0.0f);
	if (shield == 0.0f) OnShieldBroken();
}
void ACreature::AdjustMaxEnerge(float value) {
	energeMax += FMath::Min(energeMax + value, 0.0f);
	energe    += FMath::Min(energe    + value, 0.0f);
}
void ACreature::AdjustMaxDamage(float value) {
	damage = FMath::Min(damage + value, 0.0f);
}
