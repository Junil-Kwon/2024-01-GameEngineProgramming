#include "Hero.h"
#include "Weapon.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AHero::AHero() {
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AHero::OnStart() {
	Super::OnStart();
}
void AHero::OnSpawn() {
	Super::OnSpawn();
}
void AHero::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Sprite
// =============================================================================================================

void AHero::UpdateSprite(float DeltaTime) {
	Super::UpdateSprite(DeltaTime);

	switch (GetSprite()) {
	case Action::Idle:   SetSpriteIndex(nullptr,  0 + int32(GetLifeTime()  * 2) % 4); break;
	case Action::Move:   SetSpriteIndex(nullptr,  4 + int32(GetLifeTime() * 10) % 6); break;
	case Action::Jump:   SetSpriteIndex(nullptr, FMath::Min(10 + int32(GetSpriteDelay() * 10), 13)); break;
	case Action::Dash:   SetSpriteIndex(nullptr, FMath::Min(14 + int32(GetSpriteDelay() * 10), 19)); break;
	case Action::Attack: break;
	case Action::Defend: break;
	case Action::Defeat: SetSpriteIndex(nullptr, FMath::Min(20 + int32(GetSpriteDelay() * 10), 23)); break;
	}
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
bool AHero::UpdateInputs(float DeltaTime) {
	if (!Super::UpdateInputs(DeltaTime)) return false;

	// Default AI
	return true;
}
void AHero::UpdateAction(float DeltaTime) {
	Super::UpdateAction(DeltaTime);

	bool condition;
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
		if (0.2f <= GetActionDelay() && !GetCharacterMovement()->IsFalling()) {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Jump, 0.2f);
		}
		break;
	case Action::Dash:
		SetSprite(Action::Dash);
		AddMovementInput(GetLookDirection());
		if (GetActionDelay() - DeltaTime == 0.0f) {
			AddEffect(Effect::Speed, 1.0f, 0.3f);
			if (GetMoveDirection() != FVector::ZeroVector) SetLookDirection(GetMoveDirection());
			if (HasWeapon()) {
				GetWeapon()->SetSpriteOpacity(nullptr, GetWeapon()->GetSpriteOpacity() - 1.0f);
				GetWeapon()->SetAction(Action::Idle);
			}
		}
		if (0.6f <= GetActionDelay()) {
			if (HasWeapon()) GetWeapon()->SetSpriteOpacity(nullptr, GetWeapon()->GetSpriteOpacity() + 1.0f);
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -GetHitboxRadius() *  0.75f, 0.0f));
			Spawn(Identifier::Dust, GetFootLocation() + FVector(0.0f, -GetHitboxRadius() * -0.75f, 0.0f));
			SetAction(Action::Idle);
			SetActionCooldown(Action::Dash,   0.5f);
			SetActionCooldown(Action::Attack, 0.1f);
			SetActionCooldown(Action::Defend, 0.1f);
		}
		condition = int32(GetActionDelay() * 10) != int32((GetActionDelay() - DeltaTime) * 10);
		condition &= !GetCharacterMovement()->IsFalling() && (GetActionDelay() < 0.5f);
		if (GetActionDelay() - DeltaTime == 0.0f || condition) Spawn(Identifier::Dust, GetFootLocation());
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
