#include "Interactor.h"
#include "Creature.h"
#include "Indicator.h"

#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AInteractor::AInteractor() {
	defaultTag += static_cast<uint8>(Tag::Floating);
	defaultTag += static_cast<uint8>(Tag::Piercing);
	SetCollisionProfileName(TEXT("Particle"));

	nameComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Name"));
	nameComponent->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));
	nameComponent->SetupAttachment(GetAnchorComponent());

	nameComponent->SetFont(GetFont(FontType::Galmuri7));
	nameComponent->SetMaterial(0, GetFontMaterial(FontType::Galmuri7));
	nameComponent->SetTextRenderColor(FColor(0, 0, 0, 255));
	nameComponent->SetWorldSize(32.0f);
	nameComponent->SetHorizontalAlignment(EHTA_Center);
	nameComponent->SetVerticalAlignment(EVRTA_TextBottom);
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AInteractor::OnStart() {
	Super::OnStart();
}
void AInteractor::OnSpawn() {
	Super::OnSpawn();

	SetSpriteIndex(nullptr, 63);
}
void AInteractor::OnDespawn() {
	Super::OnDespawn();
}

// =============================================================================================================
// Update
// =============================================================================================================

bool AInteractor::IsHiding() { return hide; }
void AInteractor::Hide(bool value) {
	hide = value;
	SetSpriteIndex(nullptr, hide ? 63 : 0);
	if (hide) nameComponent->SetText(FText::FromString(TEXT("")));
	else nameComponent->SetText(FText::FromString(ToString(parent->GetIdentifier())));
	if (parent) RefreshLocation();
}

void AInteractor::Update(float DeltaTime) {
	Super::Update(DeltaTime);

	if (!IsHiding()) RefreshLocation();
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AInteractor::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);
	if (entity == nullptr) return;

	parent = entity;
	Hide(false);
	RefreshLocation();
}





// =============================================================================================================
// Interactor
// =============================================================================================================

void AInteractor::RefreshLocation() {
	float height = parent->GetHitboxHeight() * 0.5f + 64.0f;
	if (parent->IsA(ACreature::StaticClass())) {
		AIndicator* indicator = static_cast<ACreature*>(parent)->GetIndicator();
		if (indicator && !indicator->IsHiding()) height += !parent->HasTag(Tag::Leader) ? 96.0f : 152.0f;
	}
	SetActorLocation(parent->GetActorLocation() + FVector(0.0f, 0.0f, height));
}
