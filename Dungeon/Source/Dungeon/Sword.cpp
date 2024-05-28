#include "Sword.h"
#include "Creature.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Action
// =============================================================================================================

bool ASword::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;

	FRotator rotation;
	FVector  location;
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
		if (actionDelay - DeltaTime == 0) {
			if (FMath::Rand() < 0.75f) pattern = (pattern == 0) ? 1 : 0;
			else pattern = FMath::RandRange(0, 1);
		}
		switch (pattern) {
		case 0: SetSpriteIndex(nullptr, FMath::Min( 1 + int32(actionDelay * 10),  4)); break;
		case 1: SetSpriteIndex(nullptr, FMath::Min( 5 + int32(actionDelay * 10),  8)); break;
		}
		if (actionDelay - DeltaTime == 0.0f) {
			rotation = ToRotator(parent->GetLookDirection());
			location = parent->GetActorLocation();
			location = location + ToVector(rotation) * (parent->GetHitboxRadius() * 0.5f);
			location += RotateVector(FVector(FMath::Abs(rotation.Yaw) < 90.0f ? 1.0f : -24.0f, 0.0f, 0.0f));
			SetActorLocation(location);
			SetSpriteAngle(nullptr, rotation.Yaw - 45.0f);
			SetSpriteXFlip(nullptr, false);
			parent->SetAction(Action::Idle);
			parent->SetActionCooldown(Action::Attack, 0.5f);
			if (!parent->GetCharacterMovement()->IsFalling()) parent->AddEffect(Effect::Speed, 0.75f, 0.1f);
		}
		if (0.40f <= actionDelay) {
			SetSpriteAngle(nullptr, 0);
			SetSpriteXFlip(nullptr, parent->GetSpriteXFlip());
			SetActorLocation(parent->GetHandLocation());
			SetAction(Action::Idle);
		}
		break;
	case Action::Defend:
		if (actionDelay - DeltaTime == 0.0f) {
			parent->SetAction(Action::Idle);
			parent->SetActionCooldown(Action::Defend, 0.25f);
		}
		if (0.25f <= actionDelay) {
			SetAction(Action::Idle);
		}
		break;
	}
	return true;
}
