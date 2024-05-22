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

bool AWeapon::OnInteract(AEntity* entity) {
	if (parent != nullptr) {
		RemoveTag(Tag::Floating);
		RemoveTag(Tag::Piercing);
		AddTag(Tag::Interactability);
		
		SetActorRelativeLocation(FVector::ZeroVector);
		FVector location = parent->GetActorLocation();
		location += parent->GetArrowDirection() * parent->GetHitboxRadius();
		SetActorLocation(location);
		LaunchCharacter(parent->GetArrowDirection() * 200.0f, true, true);
		parent = nullptr;
		DetachRootComponentFromParent();
	}
	if (!Super::OnInteract(entity)) return false;
	if (entity->IsA(ACreature::StaticClass())) {
		AddTag(Tag::Floating);
		AddTag(Tag::Piercing);
		RemoveTag(Tag::Interactability);

		GetCharacterMovement()->StopMovementImmediately();
		SetActorRelativeLocation(FVector(-10.0f, 0.0f, 0.0f));
		parent = static_cast<ACreature*>(entity);
		AttachToComponent(parent->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
	return true;
}
