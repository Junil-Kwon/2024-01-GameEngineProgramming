#include "Projectile.h"
#include "Creature.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AProjectile::AProjectile() {
	defaultHitboxRadius = 10.0f;
	defaultHitboxHeight = 20.0f;
	SetCollisionProfileName(TEXT("Projectile"));
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AProjectile::OnStart() {
	Super::OnStart();
}
void AProjectile::OnSpawn() {
	Super::OnSpawn();

	switch (GetIdentifier()) {
	case Identifier::Melee:
		AddTag(Tag::Floating);
		AddTag(Tag::Piercing);
		break;
	case Identifier::Arrow:
		break;
	}
}
void AProjectile::OnDespawn() {
	Super::OnDespawn();
}

// =============================================================================================================
// Update
// =============================================================================================================

void AProjectile::Update(float DeltaTime) {
	Super::Update(DeltaTime);

	switch (GetIdentifier()) {
	case Identifier::Melee:
		if (0.2f <= GetLifeTime()) Despawn();
		break;
		break;
	case Identifier::Arrow:
		if (3.0f <= GetLifeTime()) Despawn();
		break;
	}
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AProjectile::OnCollision(AEntity* entity) {
	Super::OnCollision(entity);
	ACreature* creature = static_cast<ACreature*>(entity);

	switch (GetIdentifier()) {
	case Identifier::Melee:
		if (entity->IsA(ACreature::StaticClass()) && GetGroup() != entity->GetGroup()) {
			UE_LOG(LogTemp, Log, TEXT("%s"), *ToString(creature->GetIdentifier()));
			creature->Damage(0.1f);
		}
		break;
	case Identifier::Arrow:
		if (parent == nullptr && GetGroup() != entity->GetGroup()) {
			parent = entity;
			Attach(entity);
			AddTag(Tag::Floating);
			AddTag(Tag::Piercing);
			if (entity->IsA(ACreature::StaticClass())) creature->Damage(0.1f);
		}
		break;
	}
}
