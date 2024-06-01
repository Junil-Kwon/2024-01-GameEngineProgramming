#include "Projectile.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AProjectile::AProjectile() {
	defaultHitboxRadius = 10.0f;
	defaultHitboxHeight = 20.0f;
	//defaultTag += static_cast<uint8>(Tag::Floating);
	//defaultTag += static_cast<uint8>(Tag::Piercing);
	SetCollisionProfileName(TEXT("Projectile"));
}
void AProjectile::BeginPlay() {
	Super::BeginPlay();
}

// =============================================================================================================
// Spawn
// =============================================================================================================

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
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AProjectile::OnCollision(AEntity* entity) {
	Super::OnCollision(entity);
	
	if (GetGroup() == Group::None) return;
	switch (GetIdentifier()) {
	case Identifier::Arrow:
		if (parent == nullptr && GetGroup() != entity->GetGroup()) {
			UE_LOG(LogTemp, Warning, TEXT("%d, %d, %s"), GetGroup(), entity->GetGroup(), *entity->ToString(entity->GetIdentifier()));
			Attach(entity);
			parent = entity;
			AddTag(Tag::Floating);
			AddTag(Tag::Piercing);
			
		}
		break;
	}
}
