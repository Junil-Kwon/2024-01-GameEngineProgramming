#include "Chest.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AChest::AChest() {
	defaultHitboxRadius =  50.0f;
	defaultHitboxHeight = 100.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
}
void AChest::BeginPlay() {
	Super::BeginPlay();
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AChest::OnSpawn() {
	Super::OnSpawn();

	for (int32 i = 0; i < 100; i++) lootArray.Add(Identifier::Money);
}
void AChest::OnDespawn() {
	Super::OnDespawn();

	lootArray.Empty();
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AChest::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);

	RemoveTag(Tag::Interactability);
	SetAction(Action::Move);
}





// =============================================================================================================
// Action
// =============================================================================================================

bool AChest::VerifyAction(Action value) {
	if (GetAction() == value || GetAction() == Action::Defeat ) return false;
	if (GetAction() == Action::Idle && value == Action::Move  ) return true;
	if (GetAction() == Action::Move && value == Action::Defeat) return true;
	return false;
}
bool AChest::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;

	switch (GetAction()) {
	case Action::Idle:
		break;
	case Action::Move:
		SetSpriteIndex(nullptr, FMath::Min( 1 + static_cast<int32>(actionDelay * 10),  7));
		if (actionDelay - DeltaTime == 0.0f) loot = 0;
		if (0.7f + float(loot) / 30.0f <= actionDelay) {
			FVector location  = GetActorLocation() + FVector(0.0f, 0.0f, GetHitboxHeight() * 0.2f);
			FVector direction = FVector::ZeroVector;
			float angle = FMath::RandRange(0.4f * PI, 1.6f * PI);
			direction.X = FMath::Cos(angle) * FMath::RandRange(180.0f, 400.0f);
			direction.Y = FMath::Sin(angle) * FMath::RandRange(180.0f, 400.0f);
			direction.Z = FMath::RandRange(180.0f, 240.0f);
			Spawn(lootArray[loot], location)->LaunchCharacter(direction, true, true);
			loot++;
		}
		if (lootArray.Num() <= loot) SetAction(Action::Defeat);
		break;
	case Action::Defeat:
		break;
	}
	return true;
}





// =============================================================================================================
// Chest
// =============================================================================================================

void AChest::AddLoot(Identifier value, int32 num) {
	for (int32 i = 0; i < num; i++) lootArray.Add(value);
}
