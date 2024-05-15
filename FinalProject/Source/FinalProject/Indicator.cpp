#include "Indicator.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Materials/MaterialInstanceDynamic.h"



AIndicator::AIndicator() {
	AddTag(Tag::Floating);
	AddTag(Tag::Piercing);
	SetHitbox(0.0f, 0.0f);
	lBorderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LBorder"));
	rBorderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RBorder"));
	lHealthComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LHealth"));
	rHealthComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RHealth"));
	lShieldComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LShield"));
	rShieldComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RShield"));
	iShieldComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IShield"));
	iLeaderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ILeader"));
	SetupComponent(lBorderComponent);
	SetupComponent(rBorderComponent);
	SetupComponent(lHealthComponent);
	SetupComponent(rHealthComponent);
	SetupComponent(lShieldComponent);
	SetupComponent(rShieldComponent);
	SetupComponent(iShieldComponent);
	SetupComponent(iLeaderComponent);
	lBorderComponent->SetWorldScale3D(FVector(0.04f, 1.28f, 1.28f));
	rBorderComponent->SetWorldScale3D(FVector(0.04f, 1.28f, 1.28f));
}
void AIndicator::SetupComponent(UStaticMeshComponent* component) {
	component->SetWorldRotation(FRotator(0.0f, 90.0f, 41.409618f));
	component->SetWorldScale3D(FVector(1.28f, 1.28f, 1.28f));
	component->SetStaticMesh(GetPlaneMesh());
	component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	component->SetupAttachment(RootComponent);
}



void AIndicator::BeginPlay() {
	Super::BeginPlay();

	SetIndex(nullptr, 63);
	SetMaterial(lBorderComponent);
	SetMaterial(rBorderComponent);
	SetMaterial(lHealthComponent);
	SetMaterial(rHealthComponent);
	SetMaterial(lShieldComponent);
	SetMaterial(rShieldComponent);
	SetMaterial(iShieldComponent);
	SetMaterial(iLeaderComponent);

	SetShield(false);
	SetLeader(false);
	SetGroup(Group::None);
	SetColor(lShieldComponent, FVector(0.1f, 0.1f, 0.1f));
	SetColor(iShieldComponent, FVector(0.1f, 0.1f, 0.1f));
}


void AIndicator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}



void AIndicator::SetWidth(float value) {
	width = value;
	float i = shield ? 26.0f : 0.0f;
	lBorderComponent->SetWorldLocation(FVector(0.0f, i + width * -2.0f -  2.0f, 0.0f));
	rBorderComponent->SetWorldLocation(FVector(0.0f, i + width *  2.0f +  2.0f, 0.0f));
	iShieldComponent->SetWorldLocation(FVector(0.0f, i + width * -2.0f - 34.0f, 0.0f));
	SetHealthRatio(-1);
	SetShieldRatio(-1);
}
void AIndicator::SetGroup(Group value) {
	FVector color = FVector::OneVector;
	switch (value) {
	case Group::Friendly: color = FVector(0.031896, 0.332452, 0.152926); break;
	case Group::Enemy:    color = FVector(0.332452, 0.044270, 0.064128); break;
	}
	SetColor(lHealthComponent, color);
}

void AIndicator::SetShield(bool enable) {
	if (shield == enable) return;
	shield = enable;
	SetIndex(lBorderComponent, shield ?  1 :  0);
	SetIndex(rBorderComponent, shield ?  1 :  0);
	SetIndex(lHealthComponent, shield ?  5 :  4);
	SetIndex(rHealthComponent, shield ?  9 :  8);
	SetIndex(lShieldComponent, shield ?  4 : 63);
	SetIndex(rShieldComponent, shield ?  8 : 63);
	SetIndex(iShieldComponent, shield ? 12 : 63);
	SetWidth(width);
}
void AIndicator::SetLeader(bool enable) {
	if (leader == enable) return;
	leader = enable;
	SetIndex(iLeaderComponent, leader ? 13 : 63);
}

void AIndicator::SetHealthRatio(float value) {
	if (value != -1) healthRatio = FMath::Clamp(value, 0.0f, 1.0f);
	float i = shield ? 26.0f : 0.0f;
	lHealthComponent->SetWorldLocation(FVector(0.0f, i + width * (1 - healthRatio) * -2.0f, 0.0f));
	rHealthComponent->SetWorldLocation(FVector(0.0f, i + width * (    healthRatio) *  2.0f, 0.0f));
	lHealthComponent->SetWorldScale3D (FVector(width * 0.04f * (    healthRatio), 1.28f, 1.28f));
	rHealthComponent->SetWorldScale3D (FVector(width * 0.04f * (1 - healthRatio), 1.28f, 1.28f));
}
void AIndicator::SetShieldRatio(float value) {
	if (value != -1) shieldRatio = FMath::Clamp(value, 0.0f, 1.0f);
	float i = shield ? 26.0f : 0.0f;
	lShieldComponent->SetWorldLocation(FVector(0.0f, i + width * (1 - shieldRatio) * -2.0f, 0.0f));
	rShieldComponent->SetWorldLocation(FVector(0.0f, i + width * (    shieldRatio) *  2.0f, 0.0f));
	lShieldComponent->SetWorldScale3D(FVector(width * 0.04f * (    shieldRatio), 1.28f, 1.28f));
	rShieldComponent->SetWorldScale3D(FVector(width * 0.04f * (1 - shieldRatio), 1.28f, 1.28f));
}
