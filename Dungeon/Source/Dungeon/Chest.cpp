#include "Chest.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AChest::AChest() {
	defaultHitboxRadius =  40.0f;
	defaultHitboxHeight = 100.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
}
void AChest::BeginPlay() {
	Super::BeginPlay();
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
		SetSpriteIndex(nullptr, FMath::Min( 1 + static_cast<int32>(actionDelay * 10),  5));
		if (0.6 <= actionDelay) {
			for (int32 i = 0; i < 36; i++) {
				FVector vector = FVector(FMath::RandRange(-200, 200), FMath::RandRange(-200, 200), 0.0f);
				Spawn(Identifier::Money, GetActorLocation() + vector);
			}
			SetAction(Action::Defeat);
		}
		break;
	case Action::Defeat:
		break;
	}
	return true;
}

void AChest::OnInteract(AEntity* entity) {
	RemoveTag(Tag::Interactability);
	SetAction(Action::Move);
}
