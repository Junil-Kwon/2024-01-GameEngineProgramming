#include "Item.h"
#include "Creature.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AItem::AItem() {
	defaultHitboxRadius = 24.0f;
	defaultHitboxHeight = 72.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
	SetCollisionProfileName(TEXT("Particle"));
}
void AItem::BeginPlay() {
	Super::BeginPlay();
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AItem::OnSpawn() {
	Super::OnSpawn();

	switch (GetIdentifier()) {
	case Identifier::Shield:
		SetSpriteIndex(nullptr, FMath::RandRange(0, 7));
		break;
	}
}
void AItem::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AItem::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);

	if (entity->IsA(ACreature::StaticClass())) return;
	ACreature* creature = static_cast<ACreature*>(entity);
	switch (GetIdentifier()) {
	case Identifier::HealthPotion:
		// health effect
		creature->AdjustHealth(10.0f);
		Despawn();
		break;
	case Identifier::EnergePotion:
		// energe effect
		creature->AdjustEnerge(10.0f);
		Despawn();
		break;
	case Identifier::Shield:
		// shield effect
		creature->AdjustMaxShield(10.0f);
		Despawn();
		break;
	}
}
