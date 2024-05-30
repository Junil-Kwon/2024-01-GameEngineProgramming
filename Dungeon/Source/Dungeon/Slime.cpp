#include "Slime.h"
#include "Weapon.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

ASlime::ASlime() {
	defaultHitboxRadius = 40.0f;
	defaultHitboxHeight = 80.0f;
	defaultGroup = Group::Enemy;
}
void ASlime::BeginPlay() {
	Super::BeginPlay();
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void ASlime::OnSpawn() {
	Super::OnSpawn();
}
void ASlime::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Action
// =============================================================================================================

bool ASlime::VerifyAction(Action value) {
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
bool ASlime::UpdateInputs(float DeltaTime) {
	if (!Super::UpdateInputs(DeltaTime)) return false;

	// Default AI
	return true;
}
void ASlime::UpdateAction(float DeltaTime) {
	Super::UpdateAction(DeltaTime);

	bool condition;
	switch (GetAction()) {
	case Action::Idle:
		SetSpriteIndex(nullptr,  0 + int32(GetLifeTime() *  5) %  4);
		break;
	case Action::Move:
		SetSpriteIndex(nullptr,  0 + int32(GetLifeTime() *  5) %  4);
		AddMovementInput(GetMoveDirection());
		if (!HasWeapon() || GetWeapon()->GetAction() == Action::Idle) SetLookDirection(GetMoveDirection());
		break;
	case Action::Jump:
		SetSpriteIndex(nullptr, FMath::Min(10 + int32(GetActionDelay() * 10), 13));
		AddMovementInput(GetMoveDirection());
		if (0.1f <= GetActionDelay() && GetActionDelay() - DeltaTime < 0.1f) Jump();
		if (0.2f <= GetActionDelay() && !GetCharacterMovement()->IsFalling()) {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Jump, 0.1f);
		}
		break;
	case Action::Dash:
		if (GetActionDelay() - DeltaTime == 0.0f) {
			AddEffect(Effect::Speed, 1.0f, 0.3f);
			if (GetMoveDirection() != FVector::ZeroVector) SetLookDirection(GetMoveDirection());
			if (HasWeapon()) {
				GetWeapon()->SetSpriteOpacity(nullptr, GetWeapon()->GetSpriteOpacity() - 1.0f);
				GetWeapon()->SetAction(Action::Idle);
			}
		}
		SetSpriteIndex(nullptr, FMath::Min(14 + static_cast<int32>(GetActionDelay() * 10), 19));
		AddMovementInput(GetLookDirection());
		if (0.6f <= GetActionDelay()) {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Dash, 0.5f);
			if (HasWeapon()) GetWeapon()->SetSpriteOpacity(nullptr, GetWeapon()->GetSpriteOpacity() + 1.0f);
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -GetHitboxRadius() *  0.75f, 0.0f));
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -GetHitboxRadius() * -0.75f, 0.0f));
		}
		condition = int32(GetActionDelay() * 10) != int32((GetActionDelay() - DeltaTime) * 10);
		condition &= !GetCharacterMovement()->IsFalling() && (GetActionDelay() < 0.5f);
		if (GetActionDelay() - DeltaTime == 0.0f || condition) Spawn(Identifier::Dust, GetFootLocation());
		break;
	case Action::Attack:
		if (HasWeapon() && GetWeapon()->GetAction() == Action::Idle) {
			if (GetActionDelay() - DeltaTime == 0) GetWeapon()->SetAction(Action::Attack);
		}
		else {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Attack, 0.25f);
		}
		break;
	case Action::Defend:
		if (HasWeapon() && GetWeapon()->GetAction() == Action::Idle) {
			if (GetActionDelay() - DeltaTime == 0) GetWeapon()->SetAction(Action::Defend);
		}
		else {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Defend, 0.25f);
		}
		break;
	case Action::Defeat:
		 SetSpriteIndex(nullptr, FMath::Min(20 + int32(GetActionDelay() * 10), 23));
		break;
	}
}
