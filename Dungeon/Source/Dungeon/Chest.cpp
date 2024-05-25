#include "Chest.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AChest::AChest() {
	defaultHitboxRadius =  50.0f;
	defaultHitboxHeight = 100.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
}

// =============================================================================================================
// Object Pool
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

	int32 i = FMath::Clamp(int32((actionDelay - 0.7f) * 20.0f), 0, lootArray.Num() - 1);
	float tick = 0.7f + i / 20.0f;
	switch (GetAction()) {
	case Action::Idle:
		break;
	case Action::Move:
		SetSpriteIndex(nullptr, FMath::Min( 1 + static_cast<int32>(actionDelay * 10),  7));
		if (tick <= actionDelay && actionDelay - DeltaTime < tick) {
			FVector location = GetActorLocation() + FVector(0.0f, 0.0f, GetHitboxHeight() * 0.2f);
			FVector direction = FVector::ZeroVector;
			float angle = FMath::RandRange(0.5f * PI, 1.5f * PI);
			direction.X = FMath::Cos(angle) * FMath::RandRange(140.0f, 420.0f);
			direction.Y = FMath::Sin(angle) * FMath::RandRange(140.0f, 420.0f);
			direction.Z = FMath::RandRange(140.0f, 420.0f);
			Spawn(lootArray[i], location)->LaunchCharacter(direction, true, true);
		}
		if (i == lootArray.Num() - 1) SetAction(Action::Defeat);
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
