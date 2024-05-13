#include "Indicator.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"



AIndicator::AIndicator() {
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
}



void AIndicator::BeginPlay() {
	Super::BeginPlay();

	AddTag(Tag::Floating);
	AddTag(Tag::Piercing);
	SetShield(false);
	SetLeader(false);
	SetGroup(Group::None);
	lShieldComponent->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.1f, 0.1f, 0.1f));
	iShieldComponent->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.1f, 0.1f, 0.1f));
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
	lHealthComponent->SetVectorParameterValueOnMaterials(TEXT("Color"), color);
}

void AIndicator::SetShield(bool enable) {
	if (shield == enable) return;
	shield = enable;
	lBorderComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), shield ?  2 : 1);
	rBorderComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), shield ?  2 : 1);
	lHealthComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), shield ?  6 : 5);
	rHealthComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), shield ? 10 : 9);
	lShieldComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), shield ?  5 : 0);
	rShieldComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), shield ?  9 : 0);
	iShieldComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), shield ? 13 : 0);
	SetWidth(width);
}
void AIndicator::SetLeader(bool enable) {
	if (leader == enable) return;
	leader = enable;
	iLeaderComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), leader ? 14 : 0);
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
