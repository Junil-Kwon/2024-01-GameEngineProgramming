#include "Sword.h"
#include "Creature.h"





// =============================================================================================================
// Action
// =============================================================================================================

bool ASword::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;

	FRotator rotation;
	FVector  location;
	const float attackDuration = 0.25f;
	switch (GetAction()) {
	case Action::Idle:
		SetSpriteIndex(nullptr, 0);
		SetSpriteAngle(nullptr, 0);
		if (parent == nullptr) {

		}
		else {
			if (GetSpriteXFlip() != parent->GetSpriteXFlip()) {
				SetSpriteXFlip(nullptr, parent->GetSpriteXFlip());
				SetActorLocation(parent->GetHandLocation());
			}
		}
		break;
	case Action::Attack:
		SetSpriteIndex(nullptr, FMath::Min(1 + int32(actionDelay * 4 / attackDuration), 4));
		if (actionDelay - DeltaTime == 0.0f) {
			SetSpriteXFlip(nullptr, false);
			parent->SetAction(Action::Idle);
			parent->SetActionCooldown(Action::Attack, 0.5f);
		}

		// NEED TO FIX THIS
		// =====================================================================================================
		rotation = ToRotator(parent->GetLookDirection());
		rotation.Yaw += -60.0f + 120.0f * (actionDelay - DeltaTime) / attackDuration;
		location = parent->GetActorLocation() + ToVector(rotation) * (parent->GetHitboxRadius() + 0.0f);
		location.Z += parent->GetHandLocation().Z - parent->GetActorLocation().Z;
		SetActorLocation(location);
		SetSpriteAngle(nullptr, rotation.Yaw - 45.0f);
		// =====================================================================================================

		if (attackDuration <= actionDelay) {
			SetSpriteXFlip(nullptr, parent->GetSpriteXFlip());
			SetActorLocation(parent->GetHandLocation());
			SetAction(Action::Idle);
		}
		break;
	case Action::Defend:
		SetSpriteIndex(nullptr, FMath::Min(1 + int32(actionDelay * 20), 4));
		if (actionDelay - DeltaTime == 0.0f) {
			SetSpriteXFlip(nullptr, false);
			parent->SetAction(Action::Idle);
			parent->SetActionCooldown(Action::Defend, 1.0f);
		}
		if (0.25f <= actionDelay) {
			SetAction(Action::Idle);
		}
		break;
	}
	return true;
}
