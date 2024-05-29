#include "Weapon.h"
#include "Creature.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AWeapon::AWeapon() {
	defaultHitboxRadius = 24.0f;
	defaultHitboxHeight = 72.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
	SetCollisionProfileName(TEXT("Particle"));
}
void AWeapon::BeginPlay() {
	Super::BeginPlay();
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AWeapon::OnSpawn() {
	Super::OnSpawn();

	SetSpriteXFlip(nullptr, bool(FMath::RandRange(0, 1)));
}
void AWeapon::OnDespawn() {
	Super::OnDespawn();

	parent = nullptr;
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AWeapon::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);

	if (parent == entity || (entity != nullptr && !entity->IsA(ACreature::StaticClass()))) return;
	ACreature* creature = static_cast<ACreature*>(entity);
	if (parent != nullptr) {
		SetHitbox(defaultHitboxRadius, defaultHitboxHeight);
		RemoveTag(Tag::Floating);
		RemoveTag(Tag::Piercing);
		AddTag(Tag::Interactability);
		SetAction(Action::Idle);
	}
	if (creature != nullptr) {
		SetHitbox(0, 0);
		AddTag(Tag::Floating);
		AddTag(Tag::Piercing);
		RemoveTag(Tag::Interactability);
	}
	parent = creature;
}





// =============================================================================================================
// Action
// =============================================================================================================

bool AWeapon::VerifyAction(Action value) {
	if (!Super::VerifyAction(value)) return false;

	if (GetAction() == Action::Idle && (value == Action::Attack || value == Action::Defend)) return true;
	if (value == Action::Idle && (GetAction() == Action::Attack || GetAction() == Action::Defend)) return true;
	return false;
}
