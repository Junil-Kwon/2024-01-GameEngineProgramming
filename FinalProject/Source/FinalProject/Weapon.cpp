#include "Weapon.h"
#include "Creature.h"

#include "GameFramework/CharacterMovementComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AWeapon::AWeapon() {
	defaultHitboxRadius = 24.0f;
	defaultHitboxHeight = 72.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
}
void AWeapon::BeginPlay() {
	Super::BeginPlay();
	SetCollisionProfileName(TEXT("Collectable"));
}

// =============================================================================================================
// Update
// =============================================================================================================

void AWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}





// =============================================================================================================
// Action
// =============================================================================================================

bool AWeapon::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;
	if (parent == nullptr) return false;

	if (GetSpriteXFlip() != parent->GetSpriteXFlip()) {
		SetSpriteXFlip(nullptr, parent->GetSpriteXFlip());
		SetActorLocation(parent->GetHandLocation());
	}
	
	return true;
}

bool AWeapon::OnInteract(AEntity* entity) {
	if (parent != nullptr) {
		RemoveTag(Tag::Floating);
		RemoveTag(Tag::Piercing);
		AddTag(Tag::Interactability);
		
		SetActorLocation(parent->GetActorLocation() + parent->GetArrowDirection() * parent->GetHitboxRadius());
		LaunchCharacter(parent->GetArrowDirection() * 200.0f + FVector(0.0f, 0.0f, 200.0f), true, true);
		parent = nullptr;
		DetachRootComponentFromParent();
	}
	if (!Super::OnInteract(entity)) return false;
	if (entity->IsA(ACreature::StaticClass())) {
		AddTag(Tag::Floating);
		AddTag(Tag::Piercing);
		RemoveTag(Tag::Interactability);

		GetCharacterMovement()->StopMovementImmediately();
		parent = static_cast<ACreature*>(entity);
		AttachToComponent(parent->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		SetActorLocation(parent->GetHandLocation());
	}
	return true;
}
