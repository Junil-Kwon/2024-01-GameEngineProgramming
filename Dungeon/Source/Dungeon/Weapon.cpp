#include "Weapon.h"
#include "Creature.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AWeapon::AWeapon() {
	defaultHitboxRadius = 24.0f;
	defaultHitboxHeight = 72.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
	defaultWeaponRange = 0.0f;
	SetCollisionProfileName(TEXT("Particle"));
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AWeapon::OnStart() {
	Super::OnStart();
}
void AWeapon::OnSpawn() {
	Super::OnSpawn();

	action = Action::Idle;
	weaponRange = defaultWeaponRange;
	SetSpriteXFlip(nullptr, bool(FMath::RandRange(0, 1)));
}
void AWeapon::OnDespawn() {
	Super::OnDespawn();

	parent = nullptr;
}

// =============================================================================================================
// Update
// =============================================================================================================

void AWeapon::Update(float DeltaTime) {
	Super::Update(DeltaTime);

	float multiplier = (HasEffect(Effect::Stun) ? 0.0f : 1.0f) * (1.0f - GetEffectStrength(Effect::Freeze));
	float multiplierParent = 1.0f;
	if (parent) {
		multiplierParent *= parent->HasEffect(Effect::Stun) ? 0.0f : 1.0f;
		multiplierParent *= 1.0f - parent->GetEffectStrength(Effect::Freeze);
	}
	if (multiplier && multiplierParent) UpdateAction(DeltaTime * FMath::Min(multiplier, multiplierParent));
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AWeapon::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);

	if (parent == entity || (entity != nullptr && !entity->IsA(ACreature::StaticClass()))) return;
	ACreature* creature = static_cast<ACreature*>(entity);
	if (parent != nullptr) {
		SetSpriteOpacity(nullptr);
		SetHitbox(defaultHitboxRadius, defaultHitboxHeight);
		SetGroup(Group::None);
		RemoveTag(Tag::Floating);
		RemoveTag(Tag::Piercing);
		AddTag(Tag::Interactability);
		SetAction(Action::Idle);
	}
	if (creature != nullptr) {
		SetHitbox(0.0f, 0.0f);
		SetGroup(creature->GetGroup());
		AddTag(Tag::Floating);
		AddTag(Tag::Piercing);
		RemoveTag(Tag::Interactability);
	}
	parent = creature;
}





// =============================================================================================================
// Action
// =============================================================================================================

Action AWeapon::GetAction() {
	return action;
}
bool AWeapon::SetAction(Action value) {
	if ((parent && parent->GetActionCooldown(value)) || !VerifyAction(value)) return false;
	action = value;
	actionDelay = 0.0f;
	return true;
}
float AWeapon::GetActionDelay() {
	return actionDelay;
}
void AWeapon::SetActionDelay(float value) {
	actionDelay = value;
}
bool AWeapon::VerifyAction(Action value) {
	if (GetAction() == value) return false;
	if (value != Action::Idle || value != Action::Attack || value != Action::Defend) return false;
	return true;
}
void AWeapon::UpdateAction(float DeltaTime) {
	actionDelay += DeltaTime;
}

// =============================================================================================================
// Weapon
// =============================================================================================================

float AWeapon::GetWeaponRange() {
	return weaponRange + (parent ? (parent->GetHitboxRadius() * 0.5f) : 0.0f);
}

FVector AWeapon::GetAngleLocation(float angle) {
	if (!parent) return GetActorLocation();
	FVector location;
	location.X = FMath::Abs(angle) < 60.0f ? 1.0f : -24.0f;
	location.Y = FMath::Sin(FMath::DegreesToRadians(angle)) * parent->GetHitboxRadius() * 0.5f;
	location.Z = FMath::Cos(FMath::DegreesToRadians(angle)) * parent->GetHitboxRadius() * 0.5f;
	location = parent->GetActorLocation() + RotateVector(location);
	return location;
}