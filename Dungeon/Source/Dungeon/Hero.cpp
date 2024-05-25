#include "Hero.h"
#include "Weapon.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AHero::AHero() {
}





// =============================================================================================================
// Action
// =============================================================================================================

bool AHero::VerifyAction(Action value) {
	if (GetAction() == value || GetAction() == Action::Defeat) return false;
	if (value == Action::Idle || value == Action::Move || value == Action::Defeat) return true;
	switch (GetAction()) {
	case Action::Idle: return (GetAction() != value);
	case Action::Move: return (GetAction() != value);
	case Action::Jump:
		switch (value) {
		case Action::Jump:   return false;
		case Action::Dash:   return false;
		case Action::Attack: return true;
		case Action::Defend: return true;
		}
		break;
	case Action::Dash:
		switch (value) {
		case Action::Jump:   return false;
		case Action::Dash:   return false;
		case Action::Attack: return false;
		case Action::Defend: return false;
		}
		break;
	case Action::Attack:
		switch (value) {
		case Action::Jump:   return true;
		case Action::Dash:   return true;
		case Action::Attack: return true;
		case Action::Defend: return true;
		}
		break;
	case Action::Defend:
		switch (value) {
		case Action::Jump:   return true;
		case Action::Dash:   return true;
		case Action::Attack: return true;
		case Action::Defend: return true;
		}
		break;
	}
	return false;
}
bool AHero::UpdateInputs(float DeltaTime) {
	if (!Super::UpdateInputs(DeltaTime)) return false;

	// Default AI
	return true;
}
bool AHero::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;

	bool condition = false;
	switch (GetAction()) {
	case Action::Idle:
		SetSpriteIndex(nullptr,  0 + static_cast<int32>(actionDelay *  2) % 4);
		break;
	case Action::Move:
		SetSpriteIndex(nullptr,  4 + static_cast<int32>(actionDelay * 10) % 6);
		SetLookDirection(GetMoveDirection());
		AddMovementInput(GetMoveDirection());
		break;
	case Action::Jump:
		SetSpriteIndex(nullptr, FMath::Min(10 + static_cast<int32>(actionDelay * 10), 13));
		AddMovementInput(GetMoveDirection());
		if (0.1f <= actionDelay && actionDelay - DeltaTime < 0.1f) Jump();
		if (0.2f <= actionDelay && !GetCharacterMovement()->IsFalling()) {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Jump, 0.1f);
		}
		break;
	case Action::Dash:
		SetSpriteIndex(nullptr, FMath::Min(14 + static_cast<int32>(actionDelay * 10), 19));
		AddMovementInput(GetLookDirection());
		condition = int32(actionDelay * 10) != int32((actionDelay - DeltaTime) * 10);
		condition &= !GetCharacterMovement()->IsFalling() && (actionDelay < 0.5f);
		if (actionDelay - DeltaTime == 0.0f || condition) Spawn(Identifier::Dust, GetFootLocation());
		if (actionDelay - DeltaTime == 0.0f) AddEffect(Effect::Speed, 1.0f, 0.3f);
		if (0.6f <= actionDelay) {
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -GetHitboxRadius() *  0.75f, 0.0f));
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -GetHitboxRadius() * -0.75f, 0.0f));
			SetAction(Action::Idle);
			SetActionCooldown(Action::Dash, 0.5f);
		}
		break;
	case Action::Attack:
		SetAction(Action::Idle);
		break;
	case Action::Defend:
		SetAction(Action::Idle);
		break;
	case Action::Defeat:
		SetSpriteIndex(nullptr, FMath::Min(20 + static_cast<int32>(actionDelay * 10), 23));
		break;
	}
	return true;
}
