#include "Monster.h"
#include "Weapon.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AMonster::AMonster() {
	defaultHitboxRadius = 40.0f;
	defaultHitboxHeight = 80.0f;
	defaultGroup = Group::Enemy;
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AMonster::OnStart() {
	Super::OnStart();
}
void AMonster::OnSpawn() {
	Super::OnSpawn();
}
void AMonster::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Sprite
// =============================================================================================================

void AMonster::UpdateSprite(float DeltaTime) {
	Super::UpdateSprite(DeltaTime);

	switch (GetSprite()) {
	case Action::Idle:   SetSpriteIndex(nullptr, 0 + int32(GetLifeTime() *  2) % 4); break;
	case Action::Move:   SetSpriteIndex(nullptr, 4 + int32(GetLifeTime() * 10) % 4); break;
	case Action::Jump:   SetSpriteIndex(nullptr, 0); break;
	case Action::Dash:   SetSpriteIndex(nullptr, 0); break;
	case Action::Attack: break;
	case Action::Defend: break;
	case Action::Defeat: SetSpriteIndex(nullptr, FMath::Min(8 + int32(GetSpriteDelay() * 10), 9)); break;
	}
}

// =============================================================================================================
// Action
// =============================================================================================================

bool AMonster::VerifyAction(Action value) {
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
bool AMonster::UpdateInputs(float DeltaTime) {
	if (!Super::UpdateInputs(DeltaTime)) return false;

	if (int32(GetLifeTime()) * 10 != int32((GetLifeTime() - DeltaTime)) * 10) SearchTarget();
	if (HasTarget()) {
		float distance = FVector::Dist(GetTarget()->GetActorLocation(), GetActorLocation());
		distance -= GetTarget()->GetHitboxRadius();
		distance -= GetHitboxRadius();
		if (distance < GetAttackRange() || (HasWeapon() && distance < GetWeapon()->GetWeaponRange())) {
			SetMoveDirection(FVector::ZeroVector);
			SetAction(Action::Attack);
		}
		else {
			FVector direction = GetTarget()->GetActorLocation() - GetActorLocation();
			direction.Normalize();
			SetMoveDirection(direction);
		}
		if (GetTarget()->GetAction() == Action::Defeat) SetTarget(nullptr);
	}
	if (GetAction() == Action::Idle && !GetMoveDirection().IsZero()) SetAction(Action::Move);
	if (GetAction() == Action::Move &&  GetMoveDirection().IsZero()) SetAction(Action::Idle);
	return true;
}
void AMonster::UpdateAction(float DeltaTime) {
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
		if (0.1f <= GetActionDelay() && GetActionDelay() - DeltaTime < 0.1f) Jump();
		if (0.2f <= GetActionDelay() && !IsFalling()) {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Jump, 0.1f);
		}
		break;
	case Action::Dash:
		SetAction(Action::Idle);
		break;
	case Action::Attack:
		if (HasWeapon() && GetWeapon()->GetAction() == Action::Idle) {
			if (GetActionDelay() - DeltaTime == 0.0f) GetWeapon()->SetAction(Action::Attack);
		}
		else {
			SetAction(Action::Idle);
			if (HasTarget()) Damage(GetTarget(), GetDamage());
			SetActionCooldown(Action::Attack, GetAttackDelay());
		}
		break;
	case Action::Defend:
		if (HasWeapon() && GetWeapon()->GetAction() == Action::Idle) {
			if (GetActionDelay() - DeltaTime == 0.0f) GetWeapon()->SetAction(Action::Defend);
		}
		else {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Defend, 0.25f);
		}
		break;
	case Action::Defeat:
		SetSprite(Action::Defeat);
		break;
	}
}
