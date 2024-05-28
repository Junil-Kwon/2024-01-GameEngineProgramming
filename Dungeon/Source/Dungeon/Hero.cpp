#include "Hero.h"
#include "Weapon.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AHero::AHero() {
}
void AHero::BeginPlay() {
	Super::BeginPlay();
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AHero::OnSpawn() {
	Super::OnSpawn();
}
void AHero::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Action
// =============================================================================================================

bool AHero::VerifyAction(Action value) {
	if (!Super::VerifyAction(value)) return false;

	if (value == Action::Idle || value == Action::Move || value == Action::Defeat) return true;
	switch (GetAction()) {
	case Action::Idle: return true;
	case Action::Move: return true;
	case Action::Jump:
		switch (value) {
		case Action::Dash:   return false;
		case Action::Attack: return true;
		case Action::Defend: return true;
		}
		break;
	case Action::Dash:
		switch (value) {
		case Action::Jump:   return false;
		case Action::Attack: return false;
		case Action::Defend: return false;
		}
		break;
	case Action::Attack:
		switch (value) {
		case Action::Jump:   return true;
		case Action::Dash:   return true;
		case Action::Defend: return false;
		}
		break;
	case Action::Defend:
		switch (value) {
		case Action::Jump:   return true;
		case Action::Dash:   return true;
		case Action::Attack: return false;
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

	bool condition;
	switch (GetAction()) {
	case Action::Idle:
		SetSpriteIndex(nullptr, 0 + int32(actionDelay * 2) % 4);
		break;
	case Action::Move:
		SetSpriteIndex(nullptr, 4 + int32(actionFrame * 10) % 6);
		AddMovementInput(GetMoveDirection());
		if (!HasWeapon() || GetWeapon()->GetAction() == Action::Idle) SetLookDirection(GetMoveDirection());
		break;
	case Action::Jump:
		SetSpriteIndex(nullptr, FMath::Min(10 + int32(actionDelay * 10), 13));
		AddMovementInput(GetMoveDirection());
		if (0.1f <= actionDelay && actionDelay - DeltaTime < 0.1f) Jump();
		if (0.2f <= actionDelay && !GetCharacterMovement()->IsFalling()) {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Jump, 0.1f);
		}
		break;
	case Action::Dash:
		if (actionDelay - DeltaTime == 0.0f) {
			AddEffect(Effect::Speed, 1.0f, 0.3f);
			if (GetMoveDirection() != FVector::ZeroVector) SetLookDirection(GetMoveDirection());
			if (HasWeapon()) {
				GetWeapon()->SetSpriteOpacity(nullptr, GetWeapon()->GetSpriteOpacity() - 1.0f);
				GetWeapon()->SetAction(Action::Idle);
			}
		}
		SetSpriteIndex(nullptr, FMath::Min(14 + static_cast<int32>(actionDelay * 10), 19));
		AddMovementInput(GetLookDirection());
		if (0.6f <= actionDelay) {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Dash, 0.5f);
			if (HasWeapon()) GetWeapon()->SetSpriteOpacity(nullptr, GetWeapon()->GetSpriteOpacity() + 1.0f);
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -GetHitboxRadius() *  0.75f, 0.0f));
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -GetHitboxRadius() * -0.75f, 0.0f));
		}
		condition = int32(actionDelay * 10) != int32((actionDelay - DeltaTime) * 10);
		condition &= !GetCharacterMovement()->IsFalling() && (actionDelay < 0.5f);
		if (actionDelay - DeltaTime == 0.0f || condition) Spawn(Identifier::Dust, GetFootLocation());
		break;
	case Action::Attack:
		if (HasWeapon() && GetWeapon()->GetAction() == Action::Idle) {
			if (actionDelay - DeltaTime == 0) GetWeapon()->SetAction(Action::Attack);
		}
		else {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Attack, 0.25f);
		}
		break;
	case Action::Defend:
		if (HasWeapon() && GetWeapon()->GetAction() == Action::Idle) {
			if (actionDelay - DeltaTime == 0) GetWeapon()->SetAction(Action::Defend);
		}
		else {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Defend, 0.25f);
		}
		break;
	case Action::Defeat:
		 SetSpriteIndex(nullptr, FMath::Min(20 + int32(actionDelay * 10), 23));
		break;
	}
	return true;
}
