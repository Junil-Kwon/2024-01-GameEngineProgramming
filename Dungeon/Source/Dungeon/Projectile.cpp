#include "Projectile.h"
#include "Creature.h"

#include "GameFramework/CharacterMovementComponent.h"





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
	case Identifier::Arrow:
		if (parent == nullptr) {
			if (entity && GetGroup() != entity->GetGroup()) {
				parent = entity;
				Attach(entity);
				AddTag(Tag::Floating);
				AddTag(Tag::Piercing);
				if (entity->IsA(ACreature::StaticClass())) creature->Damage(0.1f);
			}
			else if (!entity) {
				parent = this;
				GetCharacterMovement()->StopMovementImmediately();
				AddTag(Tag::Floating);
				AddTag(Tag::Piercing);
			}
		}
		break;
	}
}
