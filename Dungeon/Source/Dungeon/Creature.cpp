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
	defaultHitboxRadius   =  36.0f;
	defaultHitboxHeight   = 108.0f;
	defaultHandLocation   = FVector2D(24.0f, -12.0f);
	defaultSensorRange    = 480.0f;
	defaultMagnetRange    = 120.0f;
	defaultIndicatorWidth =  20.0f;
	defaultHealth         =   1.0f;
	defaultArmour         =   0.0f;
	defaultEnerge         =   0.0f;
	defaultDamage         =   0.0f;
	
	sensorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sensor"));
	sensorComponent->InitCapsuleSize(0.5f, 0.5f);
	sensorComponent->SetCollisionProfileName(TEXT("Sensor"));
	sensorComponent->SetupAttachment(RootComponent);

	magnetComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Magnet"));
	magnetComponent->InitCapsuleSize(0.5f, 0.5f);
	magnetComponent->SetCollisionProfileName(TEXT("Sensor"));
	magnetComponent->SetupAttachment(RootComponent);
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void ACreature::OnStart() {
	Super::OnStart();

	sensorComponent->OnComponentBeginOverlap.AddDynamic(this, &ACreature::OnSensorBeginOverlap);
	sensorComponent->OnComponentEndOverlap  .AddDynamic(this, &ACreature::OnSensorEndOverlap  );
	magnetComponent->OnComponentBeginOverlap.AddDynamic(this, &ACreature::OnMagnetBeginOverlap);
	magnetComponent->OnComponentEndOverlap  .AddDynamic(this, &ACreature::OnMagnetEndOverlap  );
}

void ACreature::OnSpawn() {
	GetGhost()->GetCreatures()->Add(this);

	SetSensorRange(defaultSensorRange);
	SetMagnetRange(defaultMagnetRange);
	sensorArray.Empty();
	magnetArray.Empty();

	action = Action::Idle;
	for (uint8 i = 0; i < static_cast<uint8>(Action::Length); i++) actionCooldown[i] = 0.0f;
	actionDelay = 0.0f;

	health = defaultHealth, healthMax = defaultHealth;
	armour = defaultArmour, armourMax = defaultArmour;
	energe = defaultEnerge, energeMax = defaultEnerge;
	damage = defaultDamage;
	hurtCooldown = 0.0f;
	mendCooldown = 0.0f;

	Super::OnSpawn();

	SetLifeTime(FMath::RandRange(0.0f, 1.0f));

	SetIndicatorWidth(defaultIndicatorWidth);
	indicator = static_cast<AIndicator*>(Spawn(Identifier::Indicator));
	indicator->SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, GetHitboxHeight() * 0.5f + 96.0f));
	indicator->OnInteract(this);
	indicator->Attach(this);
}
void ACreature::OnDespawn() {
	Super::OnDespawn();

	GetGhost()->GetCreatures()->Remove(this);
	if (HasTag(Tag::PlayerParty)) GetGhost()->GetPlayerParty()->Remove(this);
	if (HasTag(Tag::Player     )) GetGhost()->SetPlayer(nullptr);
	SetWeapon  (nullptr);
	indicator->Despawn();
}

// =============================================================================================================
// Update
// =============================================================================================================

void ACreature::Update(float DeltaTime) {
	Super::Update(DeltaTime);

	UpdateSensor(DeltaTime);
	UpdateMagnet(DeltaTime);
	float multiplier = (HasEffect(Effect::Stun) ? 0.0f : 1.0f) * (1.0f - GetEffectStrength(Effect::Freeze));
	if (0.0f < multiplier) UpdateSprite(DeltaTime * multiplier);
	if (0.0f < multiplier) UpdateInputs(DeltaTime * multiplier);
	if (0.0f < multiplier) UpdateAction(DeltaTime * multiplier);
	if (GetAction() == Action::Defeat && 4.0f < GetLifeTime()) Despawn();
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

	if (entity && entity->HasTag(Tag::Collectable)) {
		if (GetGhost()->GetPlayer() && GetGroup() == GetGhost()->GetPlayer()->GetGroup()) {
			entity->OnInteract(this);
		}
	}
}

void ACreature::Melee(FVector location, float range, float value) {
	melee.Empty();
	TArray<ACreature*>* entity = GetGhost()->GetCreatures();
	for (int32 i = 0; i < entity->Num(); i++) {
		if ((*entity)[i] == nullptr || (*entity)[i] == this) continue;
		if ((*entity)[i]->HasTag(Tag::Invulnerability)) continue;
		if (GetGroup() != Group::None && (*entity)[i]->GetGroup() == GetGroup()) continue;
		float distance = FVector::Dist(location, (*entity)[i]->GetActorLocation());
		distance -= (*entity)[i]->GetHitboxRadius();
		if (distance < range) {
			melee.Add(static_cast<ACreature*>((*entity)[i]));
			(*entity)[i]->Damage(value);
		}
	}
}

// =============================================================================================================
// Sensor
// =============================================================================================================

void ACreature::UpdateSensor(float DeltaTime) {
	for (int32 i = sensorArray.Num() - 1; -1 < i; i--) {
		if (sensorArray[i] == nullptr) {
			sensorArray.RemoveAt(i);
			continue;
		}
	}
}

float ACreature::GetSensorRange() {
	return sensorRange;
}
void  ACreature::SetSensorRange(float value) {
	sensorRange = value;
	sensorComponent->SetCapsuleRadius(sensorRange);
}
void ACreature::OnSensorBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(ACreature::StaticClass())) {
		ACreature* creature = static_cast<ACreature*>(OtherActor);
		if (!sensorArray.Contains(creature)) sensorArray.Add(creature);
	}
}
void ACreature::OnSensorEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(ACreature::StaticClass())) {
		ACreature* creature = static_cast<ACreature*>(OtherActor);
		if (sensorArray.Contains(creature)) {
			sensorArray.Remove(creature);
			if (creature == GetTarget()) SetTarget(nullptr);
		}
	}
}

// =============================================================================================================
// Magnet
// =============================================================================================================

void ACreature::UpdateMagnet(float DeltaTime) {
	if (!HasTag(Tag::Player) && !HasTag(Tag::PlayerParty)) return;
	for (int32 i = magnetArray.Num() - 1; -1 < i; i--) {
		if (magnetArray[i] == nullptr) {
			magnetArray.RemoveAt(i);
			continue;
		}
		if (magnetArray[i]->HasTag(Tag::Collectable)) {
			magnetArray[i]->AddMovementInput(GetActorLocation() - magnetArray[i]->GetActorLocation());
		}
	}
}

float ACreature::GetMagnetRange() {
	return magnetRange;
}
void  ACreature::SetMagnetRange(float value) {
	magnetRange = value;
	magnetComponent->SetCapsuleRadius(magnetRange);
}
void ACreature::OnMagnetBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(AEntity::StaticClass())) {
		AEntity* entity = static_cast<AEntity*>(OtherActor);
		if (!magnetArray.Contains(entity)) magnetArray.Add(entity);
	}
}
void ACreature::OnMagnetEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(AEntity::StaticClass())) {
		AEntity* entity = static_cast<AEntity*>(OtherActor);
		if (magnetArray.Contains(entity)) magnetArray.Remove(entity);
	}
}

// =============================================================================================================
// Indicator
// =============================================================================================================

AIndicator* ACreature::GetIndicator() { return indicator; }
float ACreature::GetIndicatorWidth() { return indicatorWidth; }
void  ACreature::SetIndicatorWidth(float value) { indicatorWidth = value; }





// =============================================================================================================
// Target
// =============================================================================================================

bool ACreature::HasTarget() {
	return target != nullptr; 
}
ACreature* ACreature::GetTarget() {
	return target; 
}
void ACreature::SetTarget(ACreature* value) {
	if (value == target || (value != nullptr && value->HasTag(Tag::Invulnerability))) return;
	target = value;
}
void ACreature::SearchTarget(FTargetPredicateFunction match) {
	ACreature* creature = nullptr;
	float nearest = GetSensorRange();
	for (int32 i = 0; i < sensorArray.Num(); i++) {
		if (sensorArray[i] == nullptr || sensorArray[i] == this) continue;
		if (sensorArray[i]->HasTag(Tag::Invulnerability)) continue;
		if (GetGroup() != Group::None && sensorArray[i]->GetGroup() == GetGroup()) continue;
		if (match && !match(sensorArray[i])) continue;
		float distance = FVector::Dist(GetActorLocation(), sensorArray[i]->GetActorLocation());
		if (distance < nearest) {
			creature = sensorArray[i];
			nearest = distance;
		}
	}
	SetTarget(creature);
}

// =============================================================================================================
// Sprite
// =============================================================================================================

Action ACreature::GetSprite() {
	return sprite;
}
void ACreature::SetSprite(Action value) {
	if (GetSprite() == value) return;
	sprite = value;
	spriteDelay = 0.0f;
}
float ACreature::GetSpriteDelay() {
	return spriteDelay;
}
void  ACreature::SetSpriteDelay(float value) {
	spriteDelay = FMath::Max(value, 0.0f);
}

void ACreature::UpdateSprite(float DeltaTime) {
	spriteDelay += DeltaTime;
}

// =============================================================================================================
// Action
// =============================================================================================================

Action ACreature::GetAction() {
	return action;
}
bool ACreature::SetAction(Action value) {
	if (GetActionCooldown(value) || !VerifyAction(value)) return false;
	action = value;
	actionDelay = 0.0f;
	return true;
}
float ACreature::GetActionDelay() {
	return actionDelay;
}
void  ACreature::SetActionDelay(float value) {
	actionDelay = FMath::Max(value, 0.0f);
}
float ACreature::GetActionCooldown(Action value) {
	return actionCooldown[static_cast<uint8>(value)];
}
void  ACreature::SetActionCooldown(Action value, float cooldown) {
	actionCooldown[static_cast<uint8>(value)] = FMath::Max(cooldown, 0.0f);
}

bool  ACreature::VerifyAction(Action value) {
	return (GetAction() != value) ? true : false;
}
bool  ACreature::UpdateInputs(float DeltaTime) {
	// Player Action Mechanism
	if (HasTag(Tag::Player)) {
		SearchTarget();
		SetMoveDirection(GetGhost()->GetInputDirection());
		if (GetAction() == Action::Idle && !GetMoveDirection().IsZero()) SetAction(Action::Move);
		if (GetAction() == Action::Move &&  GetMoveDirection().IsZero()) SetAction(Action::Idle);
		for (uint8 i = 0; i < static_cast<uint8>(Action::Length); i++) {
			Action index = static_cast<Action>(i);
			if (GetGhost()->GetInput(index)) SetAction(index);
		}
		return false;
	}
	// Player Party Action Mechanism
	else if (HasTag(Tag::PlayerParty)) {
		if (!GetGhost()->GetPlayer()) return true;
		if (int32(GetLifeTime()) * 10 != int32((GetLifeTime() - DeltaTime)) * 10) {
			float distance = GetDistance(GetGhost()->GetPlayer(), this);
			if (distance < PlayerNearby) SetMoveDirection(FVector::ZeroVector);
			else {
				if (GetGhost()->GetPlayer()->GetSensorRange() < distance) AddEffect(Effect::Speed, 0.5f, 0.1f);
				FVector direction = GetGhost()->GetPlayer()->GetActorLocation() - GetActorLocation();
				direction.Normalize();
				SetMoveDirection(direction);
			}
		}
		if (HasWeapon()) {
			SearchTarget([](ACreature* creature) {
				float distance = creature->GetDistance(creature, creature->GetGhost()->GetPlayer());
				return distance < creature->GetGhost()->GetPlayer()->GetSensorRange() + PlayerNearby * 0.5f;
			});
			if (HasTarget()) {
				float distance = FVector::Dist(GetTarget()->GetActorLocation(), GetActorLocation());
				if (GetWeapon()->GetWeaponRange() < distance) {
					FVector direction = GetTarget()->GetActorLocation() - GetActorLocation();
					direction.Normalize();
					SetMoveDirection(direction);
				}
				else {
					SetMoveDirection(FVector::ZeroVector);
					SetAction(Action::Attack);
				}
			}
		}
		if (GetAction() == Action::Idle && !GetMoveDirection().IsZero()) SetAction(Action::Move);
		if (GetAction() == Action::Move &&  GetMoveDirection().IsZero()) SetAction(Action::Idle);
		return false;
	}
	// Etc
	return true;
}
void  ACreature::UpdateAction(float DeltaTime) {
	actionDelay += DeltaTime;
	for (uint8 i = 0; i < static_cast<uint8>(Action::Length); i++) {
		actionCooldown[i] = FMath::Max(actionCooldown[i] - DeltaTime, 0.0f);
	}
	switch (GetAction()) {
	case Action::Attack:
		if (actionDelay - DeltaTime == 0 && HasTag(Tag::Player) && GetGhost()->HasSelected()) {
			AEntity* selected = GetGhost()->GetSelected();
			if (selected->IsA(AWeapon::StaticClass())) SetWeapon(static_cast<AWeapon*>(selected));
			else selected->OnInteract(this);
			SetAction(Action::Idle);
			SetActionCooldown(Action::Attack, 0.25f);
		}
		break;
	case Action::Defend:
		break;
	}
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
// Tag
// =============================================================================================================

bool ACreature::AddTag(Tag value) {
	if (!Super::AddTag(value)) return false;
	switch (value) {
	case Tag::PlayerParty: GetGhost()->GetPlayerParty()->Add(this); break;
	case Tag::Player:      GetGhost()->SetPlayer(this); break;
	}
	return true;
}
bool ACreature::RemoveTag(Tag value) {
	if (!Super::RemoveTag(value)) return false;
	switch (value) {
	case Tag::PlayerParty: GetGhost()->GetPlayerParty()->Remove(this); break;
	case Tag::Player:      GetGhost()->SetPlayer(nullptr); break;
	}
	return true;
}

// =============================================================================================================
// Stats
// =============================================================================================================

void ACreature::Damage(float value) {
	Super::Damage(value);

	OnDamaged(value);
}

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
	float armourTemp = armour;
	if (armour) armour = FMath::Clamp(armour - value, 0.0f, armourMax);
	else        health = FMath::Clamp(health - value, 0.0f, healthMax);
	if (armour == 0.0f && armourTemp) OnArmourBroken();
	if (health == 0.0f) OnDie();
}
void  ACreature::OnArmourBroken() {
	armourMax = 0.0f;
}
void  ACreature::OnDie() {
	if (GetAction() == Action::Defeat) return;
	SetCollisionProfileName(TEXT("Particle"));
	SetAction(Action::Defeat);
	SetWeapon(nullptr);
	if (HasTag(Tag::Player)) RemoveTag(Tag::Player);

	SetLifeTime(0.0f);
	// if (HasEffect(Effect::Burn)) smoke effect, if enough strength, ashed effect
}

float ACreature::GetHealthMax() { return healthMax; }
float ACreature::GetArmourMax() { return armourMax; }
float ACreature::GetEnergeMax() { return energeMax; }
float ACreature::GetHealth() { return health; }
float ACreature::GetArmour() { return armour; }
float ACreature::GetEnerge() { return energe; }
float ACreature::GetDamage() { return damage; }

float ACreature::AdjustHealth(float value) {
	health = FMath::Clamp(health + value, 0.0f, healthMax);
	if (health == 0.0f) OnDie();
	return health;
}
float ACreature::AdjustArmour(float value) {
	armour = FMath::Clamp(armour + value, 0.0f, armourMax);
	if (armour == 0.0f) OnArmourBroken();
	return armour;
}
float ACreature::AdjustEnerge(float value) {
	energe = FMath::Clamp(energe + value, 0.0f, energeMax);
	return energe;
}

float ACreature::AdjustMaxHealth(float value) {
	healthMax += FMath::Max(healthMax + value, 0.0f);
	health    += FMath::Max(health    + value, 0.0f);
	SetIndicatorWidth(GetIndicatorWidth() * (healthMax + value) / healthMax);
	if (health == 0.0f) OnDie();
	return healthMax;
}
float ACreature::AdjustMaxArmour(float value) {
	armourMax += FMath::Max(armourMax + value, 0.0f);
	armour    += FMath::Max(armour    + value, 0.0f);
	if (armour == 0.0f) OnArmourBroken();
	return armourMax;
}
float ACreature::AdjustMaxEnerge(float value) {
	energeMax += FMath::Max(energeMax + value, 0.0f);
	energe    += FMath::Max(energe    + value, 0.0f);
	return energeMax;
}
float ACreature::AdjustMaxDamage(float value) {
	damage = FMath::Max(damage + value, 0.0f);
	return damage;
}
