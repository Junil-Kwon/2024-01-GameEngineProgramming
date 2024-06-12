#include "Knight.h"
#include "Ghost.h"
#include "Weapon.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AKnight::AKnight() {
	defaultGroup  = Group::Friendly;
	defaultIndicatorWidth = 24.0f;
	defaultHealth = 50.0f;
	defaultArmour = 50.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AKnight::OnStart() {
	Super::OnStart();

	defaultLog += "Ability : Tank";
}
void AKnight::OnSpawn() {
	Super::OnSpawn();
}
void AKnight::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Sprite
// =============================================================================================================

void AKnight::UpdateSprite(float DeltaTime) {
	Super::UpdateSprite(DeltaTime);

	switch (GetSprite()) {
	case Action::Idle:   SetSpriteIndex(nullptr,  0 + int32(GetLifeTime()  * 2) % 4); break;
	case Action::Move:   SetSpriteIndex(nullptr,  4 + int32(GetLifeTime() * 10) % 6); break;
	case Action::Jump:   SetSpriteIndex(nullptr, FMath::Min(10 + int32(GetSpriteDelay() * 10), 13)); break;
	case Action::Dash:   SetSpriteIndex(nullptr, FMath::Min(14 + int32(GetSpriteDelay() * 15), 19)); break;
	case Action::Attack: break;
	case Action::Defend: break;
	case Action::Defeat: SetSpriteIndex(nullptr, FMath::Min(20 + int32(GetSpriteDelay() * 10), 23)); break;
	}
}

// =============================================================================================================
// Action
// =============================================================================================================

bool AKnight::VerifyAction(Action value) {
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
		case Action::Defend: return (HasWeapon() ? true : false);
		}
		break;
	case Action::Defend:
		switch (value) {
		case Action::Jump:   return true;
		case Action::Dash:   return true;
		case Action::Attack: return (HasWeapon() ? true : false);
		}
		break;
	}
	return false;
}
void AKnight::UpdateAction(float DeltaTime) {
	Super::UpdateAction(DeltaTime);

	switch (GetAction()) {
	case Action::Idle:
		SetSprite(Action::Idle);
		if (HasTarget()) SetLookDirection(GetTarget()->GetActorLocation() - GetActorLocation());
		break;
	case Action::Move:
		SetSprite(Action::Move);
		AddMovementInput(GetMoveDirection());
		SetLookDirection(GetMoveDirection());
		if (HasTarget()) SetLookDirection(GetTarget()->GetActorLocation() - GetActorLocation());
		break;
	case Action::Jump:
		SetSprite(Action::Jump);
		AddMovementInput(GetMoveDirection());
		if (HasTarget()) SetLookDirection(GetTarget()->GetActorLocation() - GetActorLocation());
		if (0.1f <= GetActionDelay() && GetActionDelay() - DeltaTime < 0.1f) Jump();
		if (0.2f <= GetActionDelay() && !IsFalling()) {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Jump, 0.2f);
		}
		break;
	case Action::Dash:
		SetSprite(Action::Dash);
		AddMovementInput(GetLookDirection());
		if (GetActionDelay() - DeltaTime == 0.0f) {
			if (GetMoveDirection() != FVector::ZeroVector) SetLookDirection(GetMoveDirection());
			if (HasWeapon()) GetWeapon()->SetAction(Action::Idle);
			AddEffect(Effect::Resistance, 1.0f, 0.4f);
			AddEffect(Effect::Speed,      1.0f, 0.1f);
			Spawn(Identifier::Dust, GetFootLocation());
		}
		if (0.2f <= GetActionDelay() && GetActionDelay() - DeltaTime < 0.2f) {
			AddEffect(Effect::Slowness,   0.5f, 0.1f);
		}
		if (0.3f <= GetActionDelay() && GetActionDelay() - DeltaTime < 0.3f) {
			AddEffect(Effect::Slowness,   1.0f, 0.1f);
		}
		if (0.4f <= GetActionDelay()) {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Dash,   0.5f);
			SetActionCooldown(Action::Attack, 0.1f);
			SetActionCooldown(Action::Defend, 0.1f);
		}
		if (int32(GetActionDelay() * 10) != int32((GetActionDelay() - DeltaTime) * 10)) {
			Melee(GetActorLocation() + GetLookDirection() * GetHitboxRadius() * 0.5f, GetHitboxRadius(), 0.1f);
			for (int32 i = 0; i < melee.Num(); i++) {
				melee[i]->LaunchCharacter(GetLookDirection() * 512.0f, true, true);
			}
			if (!IsFalling() && GetActionDelay() < 0.3f) Spawn(Identifier::Dust, GetFootLocation());
		}
		break;
	case Action::Attack:
		if (HasWeapon() && GetActionDelay() - DeltaTime == 0 && GetWeapon()->SetAction(Action::Attack));
		else {
			if (SetAction(GetSprite())) SetActionDelay(GetSpriteDelay());
			SetActionCooldown(Action::Attack, 0.25f);
		}
		break;
	case Action::Defend:
		if (HasWeapon() && GetActionDelay() - DeltaTime == 0 && GetWeapon()->SetAction(Action::Defend));
		else {
			if (SetAction(GetSprite())) SetActionDelay(GetSpriteDelay());
			SetActionCooldown(Action::Defend, 0.25f);
		}
		break;
	case Action::Defeat:
		SetSprite(Action::Defeat);
		break;
	}
}
