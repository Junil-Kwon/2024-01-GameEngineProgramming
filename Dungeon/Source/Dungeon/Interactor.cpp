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
void AInteractor::BeginPlay() {
	Super::BeginPlay();

	SetCollisionProfileName(TEXT("Particle"));
	SetSpriteIndex(nullptr, 63);
	SetActive(false);
	
}





// =============================================================================================================
// Interactor
// =============================================================================================================

bool AInteractor::GetActive() {
	return active;
}
void AInteractor::SetActive(bool value) {
	active = value;
	SetSpriteIndex(nullptr, value ? 0 : 63);
	nameComponent->SetText(value ? FText::FromString(ToString(parent->GetIdentifier())) : FText::FromString(TEXT("")));
	if (parent) UpdateLocation();
}

void AInteractor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (active) UpdateLocation();
}
void AInteractor::UpdateLocation() {
	float height = parent->GetHitboxHeight() * 0.5f + 64.0f;
	if (parent->IsA(ACreature::StaticClass())) {
		if (static_cast<ACreature*>(parent)->GetIndicator()->GetActive()) {
			height += !parent->HasTag(Tag::Leader) ? 96.0f : 152.0f;
		}
	}
	SetActorLocation(parent->GetActorLocation() + FVector(0.0f, 0.0f, height));
}





// =============================================================================================================
// Action
// =============================================================================================================

void AInteractor::OnInteract(AEntity* entity) {
	if (entity == nullptr) return;
	parent = entity;
	UpdateLocation();
}
