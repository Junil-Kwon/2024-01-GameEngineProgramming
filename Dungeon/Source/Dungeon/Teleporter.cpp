#include "Teleporter.h"
#include "Ghost.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

ATeleporter::ATeleporter() {
	defaultHitboxRadius = 128.0f;
	defaultHitboxHeight = 256.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void ATeleporter::OnStart() {
	Super::OnStart();
}
void ATeleporter::OnSpawn() {
	Super::OnSpawn();

	SetSpriteIndex(nullptr, 63);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ToString(GetIdentifier()));
}
void ATeleporter::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void ATeleporter::OnCollision(AEntity* entity) {
	Super::OnCollision(entity);
}
void ATeleporter::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);

	GetGhost()->SetStage();
}
