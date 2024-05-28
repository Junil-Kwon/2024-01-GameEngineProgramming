#include "Indicator.h"
#include "Creature.h"

#include "Components/StaticMeshComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AIndicator::AIndicator() {
	defaultTag += static_cast<uint8>(Tag::Floating);
	defaultTag += static_cast<uint8>(Tag::Piercing);
	SetCollisionProfileName(TEXT("Particle"));
	
	lBorderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LBorder"));
	rBorderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RBorder"));
	lHealthComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LHealth"));
	rHealthComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RHealth"));
	lArmourComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LShield"));
	rArmourComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RShield"));
	lHBoostComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LHBoost"));
	lEnergeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LEnerge"));
	rEnergeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("REnerge"));
	iArmourComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IShield"));
	iLeaderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ILeader"));
	SetupComponent(lBorderComponent);
	SetupComponent(rBorderComponent);
	SetupComponent(lHealthComponent);
	SetupComponent(rHealthComponent);
	SetupComponent(lArmourComponent);
	SetupComponent(rArmourComponent);
	SetupComponent(lHBoostComponent);
	SetupComponent(lEnergeComponent);
	SetupComponent(rEnergeComponent);
	SetupComponent(iArmourComponent);
	SetupComponent(iLeaderComponent);
}
void AIndicator::SetupComponent(UStaticMeshComponent* component) {
	component->SetStaticMesh(GetStaticMesh(MeshType::Plane));
	component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	component->SetupAttachment(GetAnchorComponent());
}
void AIndicator::BeginPlay() {
	Super::BeginPlay();

	SetSpriteIndex(nullptr, 63);
	FVector scale = FVector::OneVector * GetTexture2D(GetIdentifier())->GetSizeX() * 0.005f;
	lBorderComponent->SetMaterial(0, GetMaterialInstanceDynamic(GetIdentifier()));
	rBorderComponent->SetMaterial(0, GetMaterialInstanceDynamic(GetIdentifier()));
	lHealthComponent->SetMaterial(0, GetMaterialInstanceDynamic(GetIdentifier()));
	rHealthComponent->SetMaterial(0, GetMaterialInstanceDynamic(GetIdentifier()));
	lArmourComponent->SetMaterial(0, GetMaterialInstanceDynamic(GetIdentifier()));
	rArmourComponent->SetMaterial(0, GetMaterialInstanceDynamic(GetIdentifier()));
	lHBoostComponent->SetMaterial(0, GetMaterialInstanceDynamic(GetIdentifier()));
	lEnergeComponent->SetMaterial(0, GetMaterialInstanceDynamic(GetIdentifier()));
	rEnergeComponent->SetMaterial(0, GetMaterialInstanceDynamic(GetIdentifier()));
	iArmourComponent->SetMaterial(0, GetMaterialInstanceDynamic(GetIdentifier()));
	iLeaderComponent->SetMaterial(0, GetMaterialInstanceDynamic(GetIdentifier()));
	lBorderComponent->SetRelativeScale3D(FVector(scale.X / 32, scale.Y, scale.Z));
	rBorderComponent->SetRelativeScale3D(FVector(scale.X / 32, scale.Y, scale.Z));
	lHealthComponent->SetRelativeScale3D(scale);
	rHealthComponent->SetRelativeScale3D(scale);
	lArmourComponent->SetRelativeScale3D(scale);
	rArmourComponent->SetRelativeScale3D(scale);
	lHBoostComponent->SetRelativeScale3D(scale);
	lEnergeComponent->SetRelativeScale3D(scale);
	rEnergeComponent->SetRelativeScale3D(scale);
	iArmourComponent->SetRelativeScale3D(scale);
	iLeaderComponent->SetRelativeScale3D(scale);
	SetSpriteColor(lHBoostComponent, FVector(1.0f, 1.0f, 1.0f));
	SetSpriteColor(lArmourComponent, FVector(0.2f, 0.2f, 0.2f));
	SetSpriteColor(iArmourComponent, FVector(0.2f, 0.2f, 0.2f));
	SetSpriteColor(lEnergeComponent, FVector(0.0f, 0.2f, 1.0f));
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void AIndicator::OnSpawn() {
	Super::OnSpawn();
	
	SetSpriteIndex(lBorderComponent, 63);
	SetSpriteIndex(rBorderComponent, 63);
	SetSpriteIndex(lHealthComponent, 63);
	SetSpriteIndex(lHBoostComponent, 63);
	SetSpriteIndex(rHealthComponent, 63);
	SetSpriteIndex(lArmourComponent, 63);
	SetSpriteIndex(rArmourComponent, 63);
	SetSpriteIndex(lEnergeComponent, 63);
	SetSpriteIndex(rEnergeComponent, 63);
	SetSpriteIndex(iArmourComponent, 63);
	SetSpriteIndex(iLeaderComponent, 63);

	width  = 0.0f;
	health = 0.0f, healthMax = 0.0f;
	armour = 0.0f, armourMax = 0.0f;
	energe = 0.0f, energeMax = 0.0f;
	hboost = 0.0f;
	group  = Group::None;
	leader = false;

	Hide(false);
}
void AIndicator::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void AIndicator::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);
	if (entity == nullptr || !entity->IsA(ACreature::StaticClass())) return;

	parent = static_cast<ACreature*>(entity);
	SetWidth ();
	SetColor ();
	SetLeader();
}





// =============================================================================================================
// Action
// =============================================================================================================

bool AIndicator::IsHiding() { return hide; }
void AIndicator::Hide(bool value) {
	hide = value;
	int32 index = (armourMax != 0.0f) + (energeMax != 0.0f);
	SetSpriteIndex(lBorderComponent, value ? 63 : index);
	SetSpriteIndex(rBorderComponent, value ? 63 : index);
	SetSpriteIndex(lHealthComponent, value ? 63 : armourMax ? 4 + 1 : 4);
	SetSpriteIndex(rHealthComponent, value ? 63 : armourMax ? 8 + 1 : 8);
	SetSpriteIndex(lArmourComponent, value ? 63 : armourMax ? 4 : 63);
	SetSpriteIndex(rArmourComponent, value ? 63 : armourMax ? 8 : 63);
	SetSpriteIndex(lHBoostComponent, value ? 63 : 4);
	SetSpriteIndex(lEnergeComponent, value ? 63 : energeMax ? 4 + index : 63);
	SetSpriteIndex(rEnergeComponent, value ? 63 : energeMax ? 8 + index : 63);
	SetSpriteIndex(iArmourComponent, value ? 63 : armourMax ? 12 : 63);
}

bool AIndicator::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime) || !parent) return false;

	if (width  != parent->GetIndicatorWidth()) SetWidth();
	else if (health != parent->GetHealth() || armourMax != parent->GetArmourMax()) SetWidth();
	else if (armour != parent->GetArmour() || armourMax != parent->GetArmourMax()) SetWidth();
	else if (energe != parent->GetEnerge() || energeMax != parent->GetEnergeMax()) SetWidth();
	else if (hboost != parent->GetEffectStrength(Effect::HealthBoost)) SetWidth();
	if (group  != parent->GetGroup()) SetColor();
	if (leader != parent->HasTag(Tag::Leader)) SetLeader();
	return true;
}





// =============================================================================================================
// Indicator
// =============================================================================================================

void AIndicator::SetWidth() {
	width  = parent->GetIndicatorWidth();
	health = parent->GetHealth();
	armour = parent->GetArmour();
	energe = parent->GetEnerge();
	healthMax = parent->GetHealthMax();
	armourMax = parent->GetArmourMax();
	energeMax = parent->GetEnergeMax();
	hboost = parent->GetEffectStrength(Effect::HealthBoost);

	int32 index = (armourMax != 0.0f) + (energeMax != 0.0f);
	SetSpriteIndex(lBorderComponent, index);
	SetSpriteIndex(rBorderComponent, index);
	SetSpriteIndex(lHealthComponent, armourMax ? 4 + 1 : 4);
	SetSpriteIndex(rHealthComponent, armourMax ? 8 + 1 : 8);
	SetSpriteIndex(lArmourComponent, armourMax ? 4 : 63);
	SetSpriteIndex(rArmourComponent, armourMax ? 8 : 63);
	SetSpriteIndex(lHBoostComponent, 4);
	SetSpriteIndex(lEnergeComponent, energeMax ? 4 + index : 63);
	SetSpriteIndex(rEnergeComponent, energeMax ? 8 + index : 63);
	SetSpriteIndex(iArmourComponent, armourMax ? 12 : 63);

	float i = armourMax ? 22.0f : 0.0f;
	float ratio = armourMax ? (armour + hboost) / armourMax : (health + hboost) / healthMax;
	ratio = 1 + FMath::Max(0.0f, ratio - 1);
	float Width = width * ratio;
	lBorderComponent->SetRelativeLocation(FVector(i + Width * -2.0f - 2.0f, 0.0f, 0.0f));
	rBorderComponent->SetRelativeLocation(FVector(i + Width *  2.0f + 2.0f, 0.0f, 0.0f));
	
	float HealthMax = (healthMax * ratio == 0.0f) ? 0.00001f : healthMax * ratio;
	float lHealthLocation = 1 - (health) / HealthMax;
	float rHealthLocation =     (health + (!armourMax ? hboost : 0.0f)) / HealthMax;
	float lHealthScale3D  =     (health) / HealthMax;
	float rHealthScale3D  = 1 - (health + (!armourMax ? hboost : 0.0f)) / HealthMax;

	lHealthComponent->SetRelativeLocation(FVector(i - lHealthLocation * Width * 2.0f, 0.0f, 0.0f));
	rHealthComponent->SetRelativeLocation(FVector(i + rHealthLocation * Width * 2.0f, 0.0f, 0.0f));
	lHealthComponent->SetRelativeScale3D (FVector(lHealthScale3D * Width * 0.04f, 1.28f, 1.28f));
	rHealthComponent->SetRelativeScale3D (FVector(rHealthScale3D * Width * 0.04f, 1.28f, 1.28f));
	
	float ArmourMax = (armourMax * ratio == 0.0f) ? 0.00001f : armourMax * ratio;
	float lShieldLocation = 1 - (armour) / ArmourMax;
	float rShieldLocation =     (armour + (armourMax ? hboost : 0.0f)) / ArmourMax;
	float lShieldScale3D  =     (armour) / ArmourMax;
	float rShieldScale3D  = 1 - (armour + (armourMax ? hboost : 0.0f)) / ArmourMax;
	lArmourComponent->SetRelativeLocation(FVector(i - lShieldLocation * Width * 2.0f, 0.0f, 0.0f));
	rArmourComponent->SetRelativeLocation(FVector(i + rShieldLocation * Width * 2.0f, 0.0f, 0.0f));
	lArmourComponent->SetRelativeScale3D (FVector(lShieldScale3D * Width * 0.04f, 1.28f, 1.28f));
	rArmourComponent->SetRelativeScale3D (FVector(rShieldScale3D * Width * 0.04f, 1.28f, 1.28f));

	float lHBoostLocation = 1 - health / healthMax - health / HealthMax;
	float lHBoostScale3D  = hboost / HealthMax;
	lHBoostComponent->SetRelativeLocation(FVector(i - lHBoostLocation * Width * 2.0f, 0.0f, 0.0f));
	lHBoostComponent->SetRelativeScale3D (FVector(lHBoostScale3D * Width * 0.04f, 1.28f, 1.28f));

	float EnergeMax = (energeMax * ratio == 0.0f) ? 0.00001f : energeMax * ratio;
	float lEnergeLocation = 1 - energe / EnergeMax;
	float rEnergeLocation =     energe / EnergeMax;
	float lEnergeScale3D  =     energe / EnergeMax;
	float rEnergeScale3D  = 1 - energe / EnergeMax;
	lEnergeComponent->SetRelativeLocation(FVector(i - lEnergeLocation * Width * 2.0f, 0.0f, 0.0f));
	rEnergeComponent->SetRelativeLocation(FVector(i + rEnergeLocation * Width * 2.0f, 0.0f, 0.0f));
	lEnergeComponent->SetRelativeScale3D (FVector(lEnergeScale3D * Width * 0.04f, 1.28f, 1.28f));
	rEnergeComponent->SetRelativeScale3D (FVector(rEnergeScale3D * Width * 0.04f, 1.28f, 1.28f));

	iArmourComponent->SetRelativeLocation(FVector(Width * -2.0f - 10.0f, 0.0f, 0.0f));
}
void AIndicator::SetColor() {
	group = parent->GetGroup();
	FVector color = FVector::OneVector;
	switch (group) {
	case Group::Friendly: color = FVector(0.031896, 0.332452, 0.152926); break;
	case Group::Enemy:    color = FVector(0.332452, 0.044270, 0.064128); break;
	default: color = FVector::OneVector; break;
	}
	SetSpriteColor(lHealthComponent, color);
}
void AIndicator::SetLeader() {
	leader = parent->HasTag(Tag::Leader);
	SetSpriteIndex(iLeaderComponent, leader ? 13 : 63);
}
