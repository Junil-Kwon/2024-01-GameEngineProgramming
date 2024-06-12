#include "Portal.h"
#include "Ghost.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

APortal::APortal() {
	defaultHitboxRadius =  64.0f;
	defaultHitboxHeight = 288.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
	defaultStage = Stage::Stage0;
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void APortal::OnStart() {
	Super::OnStart();
}
void APortal::OnSpawn() {
	Super::OnSpawn();

	stage = defaultStage;
}
void APortal::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void APortal::OnCollision(AEntity* entity) {
	Super::OnCollision(entity);
}
void APortal::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);

	GetGhost()->SetStage(stage);
}
