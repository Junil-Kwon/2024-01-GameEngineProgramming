#include "StalKnight.h"
#include "Ghost.h"
#include "Weapon.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AStalKnight::AStalKnight() {
	defaultSpeed = 360.0f;
	defaultHitboxRadius = 32.0f;
	defaultHitboxHeight = 88.0f;
	defaultHandLocation = FVector2D(24.0f, 0.0f);
	defaultGroup = Group::Friendly;
	defaultIndicatorWidth = 8.0f;
	defaultHealth = 1.0f;
	defaultArmour = 1.0f;
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AStalKnight::OnStart() {
	Super::OnStart();
}
void AStalKnight::OnSpawn() {
	Super::OnSpawn();
	
	SetAction(Action::Dash);
}
void AStalKnight::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AStalKnight::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);

	if (entity && entity->IsA(ACreature::StaticClass())) parent = static_cast<ACreature*>(entity);
}





// =============================================================================================================
// Sprite
// =============================================================================================================

void AStalKnight::UpdateSprite(float DeltaTime) {
	Super::UpdateSprite(DeltaTime);

	switch (GetSprite()) {
	case Action::Idle:   SetSpriteIndex(nullptr,  0 + int32(GetLifeTime()  * 2) % 4); break;
	case Action::Move:   SetSpriteIndex(nullptr,  4 + int32(GetLifeTime() * 10) % 6); break;
	case Action::Dash:   SetSpriteIndex(nullptr, FMath::Min(14 + int32(GetSpriteDelay() *  5), 17)); break;
	case Action::Attack: break;
	case Action::Defend: break;
	case Action::Defeat: SetSpriteIndex(nullptr, FMath::Min(10 + int32(GetSpriteDelay() * 10), 13)); break;
	}
}

// =============================================================================================================
// Action
// =============================================================================================================

bool AStalKnight::VerifyAction(Action value) {
	if (!Super::VerifyAction(value)) return false;

	if (value == Action::Idle || value == Action::Move || value == Action::Defeat) return true;
	if (value == Action::Jump) return false;
	switch (GetAction()) {
	case Action::Idle: return true;
	case Action::Move: return true;
	case Action::Attack: return (HasWeapon() ? true : false);
	case Action::Defend: return (HasWeapon() ? true : false);
	}
	return false;
}
bool AStalKnight::UpdateInputs(float DeltaTime) {
	if (!Super::UpdateInputs(DeltaTime)) return false;

	AdjustHealth(-DeltaTime * 0.1f);
	if (!parent) return false;
	if (int32(GetLifeTime()) * 10 != int32((GetLifeTime() - DeltaTime)) * 10) {
		float distance = GetDistance(parent, this);
		if (distance < PlayerNearby) SetMoveDirection(FVector::ZeroVector);
		else {
			FVector direction = parent->GetActorLocation() - GetActorLocation();
			direction.Normalize();
			SetMoveDirection(direction);
		}
		SearchTarget();
	}
	if (HasWeapon() && HasTarget()) {
		float distance = FVector::Dist(GetTarget()->GetActorLocation(), GetActorLocation());
		if (GetWeapon()->GetWeaponRange() < distance) {
			FVector direction = GetTarget()->GetActorLocation() - GetActorLocation();
			direction.Normalize();
			SetMoveDirection(direction);
		}
		else {
			SetMoveDirection(FVector::ZeroVector);
			SetAction(Action::Attack);
		}
		if (GetTarget()->GetAction() == Action::Defeat) SetTarget(nullptr);
	}
	if (GetAction() == Action::Idle && !GetMoveDirection().IsZero()) SetAction(Action::Move);
	if (GetAction() == Action::Move &&  GetMoveDirection().IsZero()) SetAction(Action::Idle);
	return true;
}
void AStalKnight::UpdateAction(float DeltaTime) {
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
	case Action::Dash:
		SetSprite(Action::Dash);
		if (GetActionDelay() - DeltaTime == 0) {
			SetSpriteXFlip  (nullptr, FMath::RandBool());
			SetSpriteOpacity(nullptr, 0.0f);
			AddEffect(Effect::Slowness, 1.0f, 0.8f);
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, GetHitboxRadius() *  0.75f, 0.0f));
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, GetHitboxRadius() * -0.75f, 0.0f));
		}
		else SetSpriteOpacity(nullptr);
		if (0.8f <= GetActionDelay()) {
			SetAction(Action::Idle);
			SetWeapon(static_cast<AWeapon*>(Spawn(Identifier::Spear)));
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