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
	defaultHitboxRadius =  36.0f;
	defaultHitboxHeight = 100.0f;
	
	sensorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sensor"));
	sensorComponent->InitCapsuleSize(defaultSensorRange, defaultHitboxHeight * 0.5f);
	sensorComponent->SetCollisionProfileName(TEXT("Sensor"));
	sensorComponent->SetupAttachment(RootComponent);

	magnetComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Magnet"));
	magnetComponent->InitCapsuleSize(defaultMagnetRange, defaultHitboxHeight * 0.5f);
	magnetComponent->SetCollisionProfileName(TEXT("Sensor"));
	magnetComponent->SetupAttachment(RootComponent);
}
void ACreature::BeginPlay() {
	SetSensorRange(defaultSensorRange);
	SetMagnetRange(defaultMagnetRange);
	
	indicator = static_cast<AIndicator*>(Spawn(Identifier::Indicator));
	indicatorWidth = defaultIndicatorWidth;

	healthMax = health;
	shieldMax = shield;
	energeMax = energe;

	Super::BeginPlay();

	indicator->OnInteract(this);

	sensorComponent->OnComponentBeginOverlap.AddDynamic(this, &ACreature::OnSensorBeginOverlap);
	sensorComponent->OnComponentEndOverlap  .AddDynamic(this, &ACreature::OnSensorEndOverlap  );
	magnetComponent->OnComponentBeginOverlap.AddDynamic(this, &ACreature::OnMagnetBeginOverlap);
	magnetComponent->OnComponentEndOverlap  .AddDynamic(this, &ACreature::OnMagnetEndOverlap  );
}
void ACreature::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (HasWeapon()) weapon->Destroy();
	indicator->Destroy();
}

// =============================================================================================================
// Update
// =============================================================================================================

void ACreature::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UpdateSensor(DeltaTime);
	UpdateMagnet(DeltaTime);
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void ACreature::OnHitboxChanged() {
	Super::OnHitboxChanged();
	sensorComponent->SetCapsuleHalfHeight(sensorRange + GetHitboxHeight() * 0.5f - GetHitboxRadius());
	magnetComponent->SetCapsuleHalfHeight(magnetRange + GetHitboxHeight() * 0.5f - GetHitboxRadius());
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
}
void ACreature::OnSensorEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
}

// =============================================================================================================
// Magnet
// =============================================================================================================

bool ACreature::UpdateMagnet(float DeltaTime) {
	if (GetGroup() != GetGhost()->GetPlayer()->GetGroup()) return false;

	for (int32 i = collectable.Num() - 1; -1 < i; i--) {
		if (collectable[i] == nullptr || !collectable[i]->HasTag(Tag::Collectable)) {
			collectable.RemoveAt(i);
			continue;
		}
		// pull other actor to this actor
	}
	if (HasTag(Tag::Player)) {
		AEntity* entity = nullptr;
		float nearest = magnetRange;
		for (int32 i = interactability.Num() - 1; -1 < i; i--) {
			if (interactability[i] == nullptr || !interactability[i]->HasTag(Tag::Interactability)) {
				interactability.RemoveAt(i);
				continue;
			}
			float distance = FVector::Distance(interactability[i]->GetActorLocation(), GetActorLocation());
			if (distance < nearest) {
				entity = interactability[i];
				nearest = distance;
			}
		}
		SetSelected(entity);
	}
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

	AEntity* entity = Cast<AEntity>(OtherActor);
	if (entity == nullptr) return;
	if (!interactability.Contains(entity) && entity->HasTag(Tag::Interactability)) interactability.Add(entity);
	if (!collectable    .Contains(entity) && entity->HasTag(Tag::Collectable    )) collectable    .Add(entity);
}
void ACreature::OnMagnetEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	AEntity* entity = Cast<AEntity>(OtherActor);
	if (entity == nullptr) return;
	if (interactability.Contains(entity)) SetSelected(nullptr);
	if (interactability.Contains(entity)) interactability.Remove(entity);
	if (collectable    .Contains(entity)) collectable    .Remove(entity);
}

// =============================================================================================================
// Select
// =============================================================================================================

bool     ACreature::HasSelected() { return selected != nullptr; };
AEntity* ACreature::GetSelected() { return selected; }
void ACreature::SetSelected(AEntity* entity) {
	if (entity == selected) return;
	if (selected != nullptr) {
		if (selected->HasTag(Tag::Interactability)) selected->GetInteractor()->SetActive(false);
		selected = nullptr;
	}
	if (entity != nullptr) {
		selected = entity;
		if (selected->HasTag(Tag::Interactability)) selected->GetInteractor()->SetActive(true);
	}
}

// =============================================================================================================
// Weapon
// =============================================================================================================

bool     ACreature::HasWeapon() { return weapon != nullptr; }
AWeapon* ACreature::GetWeapon() { return weapon; }
void ACreature::SetWeapon(AWeapon* value) {
	if (weapon != nullptr) {
		weapon->OnInteract(nullptr);
		OnMagnetBeginOverlap(nullptr, weapon, nullptr, 0, false, FHitResult());
		weapon = nullptr;
	}
	if (weapon == nullptr) {
		if (value) value->OnInteract(this);
		weapon = value;
	}
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
	switch (GetAction()) {
	case Action::Attack:
		if (HasSelected()) {
			if (GetSelected()->IsA(AWeapon::StaticClass())) SetWeapon(static_cast<AWeapon*>(GetSelected()));
			else GetSelected()->OnInteract(this);
			SetAction(Action::Idle);
			SetActionCooldown(Action::Attack, 0.2f);
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
	// refresh indicator
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
	// refresh indicator
}
void ACreature::AdjustMaxShield(float value) {
	shieldMax += FMath::Min(shieldMax + value, 0.0f);
	shield    += FMath::Min(shield    + value, 0.0f);
	if (shield == 0.0f) OnShieldBroken();
	// refresh indicator
}
void ACreature::AdjustMaxEnerge(float value) {
	energeMax += FMath::Min(energeMax + value, 0.0f);
	energe    += FMath::Min(energe    + value, 0.0f);
	// refresh indicator
}
void ACreature::AdjustMaxDamage(float value) {
	damage = FMath::Min(damage + value, 0.0f);
}
