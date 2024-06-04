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

	if (entity == nullptr || !entity->IsA(ACreature::StaticClass())) return;
	ACreature* creature = static_cast<ACreature*>(entity);

	FVector location;
	float angle;
	switch (GetIdentifier()) {
	case Identifier::HealthPotion:
		if (entity != nullptr) for (uint8 i = 0; i < 3; i++) {
			location = FVector::ZeroVector;
			angle = FMath::RandRange(0.0f * PI, 2.0f * PI);
			location.X = -4.0f;
			location.Y = entity->GetHitboxRadius() * FMath::Cos(angle) * FMath::RandRange(0.0f, 0.8f);
			location.Z = entity->GetHitboxHeight() * FMath::Sin(angle) * FMath::RandRange(0.0f, 0.4f);
			Spawn(Identifier::HealthUp, entity->GetActorLocation() + RotateVector(location))->Attach(entity);
		}
		creature->AdjustHealth(10.0f);
		Despawn();
		break;
	case Identifier::EnergePotion:
		if (!creature->GetEnergeMax()) break;
		if (entity != nullptr) for (uint8 i = 0; i < 3; i++) {
			location = FVector::ZeroVector;
			angle = FMath::RandRange(0.0f * PI, 2.0f * PI);
			location.X = -4.0f;
			location.Y = entity->GetHitboxRadius() * FMath::Cos(angle) * FMath::RandRange(0.0f, 0.8f);
			location.Z = entity->GetHitboxHeight() * FMath::Sin(angle) * FMath::RandRange(0.0f, 0.4f);
			Spawn(Identifier::EnergeUp, entity->GetActorLocation() + RotateVector(location))->Attach(entity);
		}
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
