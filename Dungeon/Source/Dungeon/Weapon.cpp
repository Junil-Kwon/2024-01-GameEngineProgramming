#include "Weapon.h"
#include "Creature.h"

#include "GameFramework/CharacterMovementComponent.h"





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
		RemoveTag(Tag::Floating);
		RemoveTag(Tag::Piercing);
		AddTag(Tag::Interactability);
	}
	if (creature != nullptr) {
		AddTag(Tag::Floating);
		AddTag(Tag::Piercing);
		RemoveTag(Tag::Interactability);
	}
	parent = creature;
}





// =============================================================================================================
// Action
// =============================================================================================================

bool AWeapon::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;
	if (parent == nullptr) return false;

	if (GetSpriteXFlip() != parent->GetSpriteXFlip()) {
		SetSpriteXFlip(nullptr, parent->GetSpriteXFlip());
		SetActorLocation(parent->GetHandLocation());
	}
	return true;
}
