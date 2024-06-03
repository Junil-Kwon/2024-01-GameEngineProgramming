#include "Item.h"
#include "Creature.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AItem::AItem() {
	defaultHitboxRadius = 30.0f;
	defaultHitboxHeight = 60.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
	SetCollisionProfileName(TEXT("Particle"));
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AItem::OnStart() {
	Super::OnStart();
}
void AItem::OnSpawn() {
	Super::OnSpawn();

	switch (GetIdentifier()) {
	case Identifier::Armour:
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

	if (!entity->IsA(ACreature::StaticClass())) return;
	ACreature* creature = static_cast<ACreature*>(entity);
	switch (GetIdentifier()) {
	case Identifier::HealthPotion:
		// health effect
		creature->AdjustHealth(10.0f);
		Despawn();
		break;
	case Identifier::EnergePotion:
		if (!creature->GetEnergeMax()) break;
		// energe effect
		creature->AdjustEnerge(10.0f);
		Despawn();
		break;
	case Identifier::Armour:
		// shield effect
		creature->AdjustMaxArmour(10.0f);
		Despawn();
		break;
	}
}
