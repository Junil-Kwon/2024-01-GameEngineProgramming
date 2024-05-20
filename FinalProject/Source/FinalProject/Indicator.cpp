#include "Indicator.h"

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
	lEnergeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LEnerge"));
	rEnergeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("REnerge"));
	hpBoostComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HPBoost"));
	iShieldComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IShield"));
	iLeaderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ILeader"));

	SetupComponent(lBorderComponent);
	SetupComponent(rBorderComponent);
	SetupComponent(lHealthComponent);
	SetupComponent(rHealthComponent);
	SetupComponent(lShieldComponent);
	SetupComponent(rShieldComponent);
	SetupComponent(lEnergeComponent);
	SetupComponent(rEnergeComponent);
	SetupComponent(hpBoostComponent);
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
	CreateMaterial(lEnergeComponent);
	CreateMaterial(rEnergeComponent);
	CreateMaterial(hpBoostComponent);
	CreateMaterial(iShieldComponent);
	CreateMaterial(iLeaderComponent);


	SetSpriteIndex(lBorderComponent,  0);
	SetSpriteIndex(rBorderComponent,  0);
	SetSpriteIndex(lHealthComponent,  4);
	SetSpriteIndex(rHealthComponent,  8);
	SetSpriteIndex(lShieldComponent, 63);
	SetSpriteIndex(rShieldComponent, 63);
	SetSpriteIndex(lEnergeComponent, 63);
	SetSpriteIndex(rEnergeComponent, 63);
	SetSpriteIndex(hpBoostComponent, 63);
	SetSpriteIndex(iShieldComponent, 63);
	SetSpriteIndex(iLeaderComponent, 63);
	
	SetWidth(width);
	SetGroup(Group::None);
	SetLeader(false);
	SetSpriteColor(lShieldComponent, FVector(0.1f, 0.1f, 0.1f));
	SetSpriteColor(iShieldComponent, FVector(0.1f, 0.1f, 0.1f));
}





// =============================================================================================================
// Indicator
// =============================================================================================================

void AIndicator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);


}

void AIndicator::SetWidth(float value) {
	width = value;
	float i = shieldRatio ? 26.0f : 0.0f;
	lBorderComponent->SetRelativeLocation(FVector(0.0f, i + width * -2.0f -  2.0f, 0.0f));
	rBorderComponent->SetRelativeLocation(FVector(0.0f, i + width *  2.0f +  2.0f, 0.0f));
	iShieldComponent->SetRelativeLocation(FVector(0.0f, i + width * -2.0f - 34.0f, 0.0f));
	SetHealthRatio();
	SetShieldRatio();
	SetEnergeRatio();
}
void AIndicator::SetGroup(Group value) {
	FVector color = FVector::OneVector;
	switch (value) {
	case Group::Friendly: color = FVector(0.031896, 0.332452, 0.152926); break;
	case Group::Enemy:    color = FVector(0.332452, 0.044270, 0.064128); break;
	}
	SetSpriteColor(lHealthComponent, color);
}
void AIndicator::SetHealthRatio(float value) {
	if (value != -1) healthRatio = FMath::Clamp(value, 0.0f, 1.0f);
	float i = shieldRatio ? 26.0f : 0.0f;
	lHealthComponent->SetRelativeLocation(FVector(0.0f, i + width * (1 - healthRatio) * -2.0f, 0.0f));
	rHealthComponent->SetRelativeLocation(FVector(0.0f, i + width * (    healthRatio) *  2.0f, 0.0f));
	lHealthComponent->SetRelativeScale3D(FVector(width * 0.04f * (    healthRatio), 1.28f, 1.28f));
	rHealthComponent->SetRelativeScale3D(FVector(width * 0.04f * (1 - healthRatio), 1.28f, 1.28f));
}
void AIndicator::SetShieldRatio(float value) {
	if (value != -1 && (shieldRatio == 0) != (value == 0)) {
		SetHealthRatio();
		SetEnergeRatio();
		int32 index = shieldRatio != 0 + energeRatio != 0;
		SetSpriteIndex(lBorderComponent, index);
		SetSpriteIndex(rBorderComponent, index);
		SetSpriteIndex(lHealthComponent, shieldRatio ? 4 : 5);
		SetSpriteIndex(rHealthComponent, shieldRatio ? 8 : 9);
		SetSpriteIndex(lShieldComponent, index != 0 ? 4 : 63);
		SetSpriteIndex(rShieldComponent, index != 0 ? 8 : 63);
		SetSpriteIndex(iShieldComponent, value ? 12 : 63);
	}
	if (value != -1) shieldRatio = FMath::Clamp(value, 0.0f, 1.0f);
	float i = shieldRatio ? 26.0f : 0.0f;
	lShieldComponent->SetRelativeLocation(FVector(0.0f, i + width * (1 - shieldRatio) * -2.0f, 0.0f));
	rShieldComponent->SetRelativeLocation(FVector(0.0f, i + width * (    shieldRatio) *  2.0f, 0.0f));
	lShieldComponent->SetRelativeScale3D(FVector(width * 0.04f * (    shieldRatio), 1.28f, 1.28f));
	rShieldComponent->SetRelativeScale3D(FVector(width * 0.04f * (1 - shieldRatio), 1.28f, 1.28f));
}
void AIndicator::SetEnergeRatio(float value) {
	if (value != -1 && (energeRatio == 0) != (value == 0)) {
		int32 index = shieldRatio != 0 + energeRatio != 0;
		SetSpriteIndex(lBorderComponent, index);
		SetSpriteIndex(rBorderComponent, index);
		SetSpriteIndex(lEnergeComponent, index + 4);
		SetSpriteIndex(rEnergeComponent, index + 8);
	}
	if (value != -1) energeRatio = FMath::Clamp(value, 0.0f, 1.0f);
	float i = shieldRatio ? 26.0f : 0.0f;
	lEnergeComponent->SetRelativeLocation(FVector(0.0f, i + width * (1 - energeRatio) * -2.0f, 0.0f));
	rEnergeComponent->SetRelativeLocation(FVector(0.0f, i + width * (    energeRatio) *  2.0f, 0.0f));
	lEnergeComponent->SetRelativeScale3D(FVector(width * 0.04f * (    energeRatio), 1.28f, 1.28f));
	rEnergeComponent->SetRelativeScale3D(FVector(width * 0.04f * (1 - energeRatio), 1.28f, 1.28f));
}
void AIndicator::SetLeader(bool value) {
	leader = value;
	SetSpriteIndex(iLeaderComponent, value ? 13 : 63);
}
