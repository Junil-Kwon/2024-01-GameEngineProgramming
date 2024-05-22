#include "Indicator.h"
#include "Creature.h"

#include "Components/StaticMeshComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AIndicator::AIndicator() {
	defaultTag += static_cast<uint8>(Tag::Floating);
	defaultTag += static_cast<uint8>(Tag::Piercing);
	
	lBorderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LBorder"));
	rBorderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RBorder"));
	lHealthComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LHealth"));
	rHealthComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RHealth"));
	lShieldComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LShield"));
	rShieldComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RShield"));
	lHBoostComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LHBoost"));
	lEnergeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LEnerge"));
	rEnergeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("REnerge"));
	iShieldComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IShield"));
	iLeaderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ILeader"));
	SetupComponent(lBorderComponent);
	SetupComponent(rBorderComponent);
	SetupComponent(lHealthComponent);
	SetupComponent(rHealthComponent);
	SetupComponent(lShieldComponent);
	SetupComponent(rShieldComponent);
	SetupComponent(lHBoostComponent);
	SetupComponent(lEnergeComponent);
	SetupComponent(rEnergeComponent);
	SetupComponent(iShieldComponent);
	SetupComponent(iLeaderComponent);
	lBorderComponent->SetRelativeScale3D(FVector(0.04f, 1.28f, 1.28f));
	rBorderComponent->SetRelativeScale3D(FVector(0.04f, 1.28f, 1.28f));
}
void AIndicator::SetupComponent(UStaticMeshComponent* component) {
	component->SetRelativeRotation(FRotator(0.0f, 90.0f, 41.409618f));
	component->SetRelativeScale3D(FVector(1.28f, 1.28f, 1.28f));
	component->SetStaticMesh(GetPlaneMesh());
	component->SetMaterial(0, GetMaterialInstance());
	component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	component->SetupAttachment(RootComponent);
}
void AIndicator::BeginPlay() {
	Super::BeginPlay();

	SetCollisionProfileName(TEXT("Particle"));
	SetSpriteIndex(nullptr, 63);
	CreateMaterial(lBorderComponent);
	CreateMaterial(rBorderComponent);
	CreateMaterial(lHealthComponent);
	CreateMaterial(rHealthComponent);
	CreateMaterial(lShieldComponent);
	CreateMaterial(rShieldComponent);
	CreateMaterial(lHBoostComponent);
	CreateMaterial(lEnergeComponent);
	CreateMaterial(rEnergeComponent);
	CreateMaterial(iShieldComponent);
	CreateMaterial(iLeaderComponent);
	SetSpriteIndex(lBorderComponent, 63);
	SetSpriteIndex(rBorderComponent, 63);
	SetSpriteIndex(lHealthComponent, 63);
	SetSpriteIndex(lHBoostComponent, 63);
	SetSpriteIndex(rHealthComponent, 63);
	SetSpriteIndex(lShieldComponent, 63);
	SetSpriteIndex(rShieldComponent, 63);
	SetSpriteIndex(lEnergeComponent, 63);
	SetSpriteIndex(rEnergeComponent, 63);
	SetSpriteIndex(iShieldComponent, 63);
	SetSpriteIndex(iLeaderComponent, 63);
	SetSpriteColor(lHBoostComponent, FVector(1.0f, 1.0f, 1.0f));
	SetSpriteColor(lShieldComponent, FVector(0.2f, 0.2f, 0.2f));
	SetSpriteColor(iShieldComponent, FVector(0.2f, 0.2f, 0.2f));
	SetSpriteColor(lEnergeComponent, FVector(0.0f, 0.2f, 1.0f));
}





// =============================================================================================================
// Indicator
// =============================================================================================================

void AIndicator::SetWidth() {
	width = parent->GetIndicatorWidth();
	health = parent->GetHealth();
	shield = parent->GetShield();
	energe = parent->GetEnerge();
	healthMax = parent->GetHealthMax();
	shieldMax = parent->GetShieldMax();
	energeMax = parent->GetEnergeMax();
	hboost = parent->GetEffectStrength(Effect::HealthBoost);

	int32 index = (shieldMax != 0.0f) + (energeMax != 0.0f);
	SetSpriteIndex(lBorderComponent, index);
	SetSpriteIndex(rBorderComponent, index);
	SetSpriteIndex(lHealthComponent, shieldMax ? 4 + 1 : 4);
	SetSpriteIndex(rHealthComponent, shieldMax ? 8 + 1 : 8);
	SetSpriteIndex(lShieldComponent, shieldMax ? 4 : 63);
	SetSpriteIndex(rShieldComponent, shieldMax ? 8 : 63);
	SetSpriteIndex(lHBoostComponent, 4);
	SetSpriteIndex(lEnergeComponent, energeMax ? 4 + index : 63);
	SetSpriteIndex(rEnergeComponent, energeMax ? 8 + index : 63);
	SetSpriteIndex(iShieldComponent, shieldMax ? 12 : 63);

	float i = shieldMax ? 22.0f : 0.0f;
	float ratio = shieldMax ? (shield + hboost) / shieldMax : (health + hboost) / healthMax;
	ratio = 1 + FMath::Max(0.0f, ratio - 1);
	float Width = width * ratio;
	lBorderComponent->SetRelativeLocation(FVector(0.0f, i + Width * -2.0f - 2.0f, 0.0f));
	rBorderComponent->SetRelativeLocation(FVector(0.0f, i + Width *  2.0f + 2.0f, 0.0f));
	
	float HealthMax = (healthMax * ratio == 0.0f) ? 0.00001f : healthMax * ratio;
	float lHealthLocation = 1 - (health) / HealthMax;
	float rHealthLocation =     (health + (!shieldMax ? hboost : 0.0f)) / HealthMax;
	float lHealthScale3D =     (health) / HealthMax;
	float rHealthScale3D = 1 - (health + (!shieldMax ? hboost : 0.0f)) / HealthMax;

	lHealthComponent->SetRelativeLocation(FVector(0.0f, i - lHealthLocation * Width * 2.0f, 0.0f));
	rHealthComponent->SetRelativeLocation(FVector(0.0f, i + rHealthLocation * Width * 2.0f, 0.0f));
	lHealthComponent->SetRelativeScale3D(FVector(lHealthScale3D * Width * 0.04f, 1.28f, 1.28f));
	rHealthComponent->SetRelativeScale3D(FVector(rHealthScale3D * Width * 0.04f, 1.28f, 1.28f));
	
	float ShieldMax = (shieldMax * ratio == 0.0f) ? 0.00001f : shieldMax * ratio;
	float lShieldLocation = 1 - (shield) / ShieldMax;
	float rShieldLocation =     (shield + (shieldMax ? hboost : 0.0f)) / ShieldMax;
	float lShieldScale3D =     (shield) / ShieldMax;
	float rShieldScale3D = 1 - (shield + (shieldMax ? hboost : 0.0f)) / ShieldMax;
	lShieldComponent->SetRelativeLocation(FVector(0.0f, i - lShieldLocation * Width * 2.0f, 0.0f));
	rShieldComponent->SetRelativeLocation(FVector(0.0f, i + rShieldLocation * Width * 2.0f, 0.0f));
	lShieldComponent->SetRelativeScale3D(FVector(lShieldScale3D * Width * 0.04f, 1.28f, 1.28f));
	rShieldComponent->SetRelativeScale3D(FVector(rShieldScale3D * Width * 0.04f, 1.28f, 1.28f));

	float lHBoostLocation = 1 - health / healthMax - health / HealthMax;
	float lHBoostScale3D = hboost / HealthMax;
	lHBoostComponent->SetRelativeLocation(FVector(0.0f, i - lHBoostLocation * Width * 2.0f, 0.0f));
	lHBoostComponent->SetRelativeScale3D(FVector(lHBoostScale3D * Width * 0.04f, 1.28f, 1.28f));

	float EnergeMax = (energeMax * ratio == 0.0f) ? 0.00001f : energeMax * ratio;
	float lEnergeLocation = 1 - energe / EnergeMax;
	float rEnergeLocation =     energe / EnergeMax;
	float lEnergeScale3D =     energe / EnergeMax;
	float rEnergeScale3D = 1 - energe / EnergeMax;
	lEnergeComponent->SetRelativeLocation(FVector(0.0f, i - lEnergeLocation * Width * 2.0f, 0.0f));
	rEnergeComponent->SetRelativeLocation(FVector(0.0f, i + rEnergeLocation * Width * 2.0f, 0.0f));
	lEnergeComponent->SetRelativeScale3D(FVector(lEnergeScale3D * Width * 0.04f, 1.28f, 1.28f));
	rEnergeComponent->SetRelativeScale3D(FVector(rEnergeScale3D * Width * 0.04f, 1.28f, 1.28f));

	iShieldComponent->SetRelativeLocation(FVector(0.0f, Width * -2.0f - 10.0f, 0.0f));
}
void AIndicator::SetGroup() {
	group = parent->GetGroup();
	FVector color = FVector::OneVector;
	switch (group) {
	case Group::Friendly: color = FVector(0.031896, 0.332452, 0.152926); break;
	case Group::Enemy:    color = FVector(0.332452, 0.044270, 0.064128); break;
	default: color = FVector::ZeroVector; break;
	}
	SetSpriteColor(lHealthComponent, color);
}
void AIndicator::SetLeader() {
	leader = parent->HasTag(Tag::Leader);
	SetSpriteIndex(iLeaderComponent, leader ? 13 : 63);
}

bool AIndicator::GetActive() {
	return active;
}
void AIndicator::SetActive(bool value) {
	active = value;
	//-
}
void AIndicator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (!parent) return;

	if (width != parent->GetIndicatorWidth()) SetWidth();
	else if (health != parent->GetHealth() || shieldMax != parent->GetShieldMax()) SetWidth();
	else if (shield != parent->GetShield() || shieldMax != parent->GetShieldMax()) SetWidth();
	else if (energe != parent->GetEnerge() || energeMax != parent->GetEnergeMax()) SetWidth();
	else if (hboost != parent->GetEffectStrength(Effect::HealthBoost)) SetWidth();
	if (group != parent->GetGroup()) SetGroup();
	if (leader != parent->HasTag(Tag::Leader)) SetLeader();
}





// =============================================================================================================
// Action
// =============================================================================================================

bool AIndicator::OnInteract(AEntity* entity) {
	if (!Super::OnInteract(entity) || !entity->IsA(ACreature::StaticClass())) return false;
	parent = static_cast<ACreature*>(entity);
	SetActorRelativeLocation(FVector(0.0f, 0.0f, parent->GetHitboxHeight() * 0.5f + 96.0f));
	AttachToComponent(parent->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SetWidth();
	SetGroup();
	SetLeader();
	SetActive(true);
	return true;
}
