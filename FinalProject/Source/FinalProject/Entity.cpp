#include "Entity.h"
#include "Particle.h"
#include "Indicator.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <type_traits>



bool AEntity::operator==(const AEntity& other) const { return this == &other; }

template<typename TEnum> FString AEntity::ToFString(TEnum value) {
	return StaticEnum<TEnum>()->GetNameStringByValue(static_cast<uint8>(value));
}
template<typename TEnum> TEnum AEntity::ToEnum(FString value) {
	int64 index = StaticEnum<TEnum>()->GetValueByName(FName(*value));
	return static_cast<TEnum>(index == INDEX_NONE ? 0 : index);
}
FRotator AEntity::ToRotator(FVector  value) {
	float degree = 90.0f - FMath::RadiansToDegrees(FMath::Atan2(value.X, value.Y));
	return FRotator(0.0f, degree, 0.0f);
}
FVector  AEntity::ToVector(FRotator value) {
	float radian = FMath::DegreesToRadians(value.Yaw);
	return FVector(FMath::Cos(radian), FMath::Sin(radian), 0.0f);
}



UStaticMesh* AEntity::GetPlaneMesh() {
	static UStaticMesh* uStaticMesh = nullptr;
	if (uStaticMesh == nullptr) {
		FString path = "/Engine/BasicShapes/Plane.Plane";
		uStaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *path));
	}
	return uStaticMesh;
}
UStaticMesh* AEntity::GetSphereMesh() {
	static UStaticMesh* uStaticMesh = nullptr;
	if (uStaticMesh == nullptr) {
		FString path = "/Engine/BasicShapes/Sphere.Sphere";
		uStaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *path));
	}
	return uStaticMesh;
}
UTexture* AEntity::GetTexture(Identifier value) {
	static UTexture* uTexture[static_cast<uint8>(Identifier::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uTexture[index] == nullptr) {
		FString name = StaticEnum<Identifier>()->GetNameStringByIndex(index);
		FString path = "/Game/Materials/" + name + "." + name;
		uTexture[index] = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), nullptr, *path));
	}
	if (!uTexture[index]) UE_LOG(LogTemp, Warning, TEXT("Texture '%s' not found"), *ToFString(value));
	return uTexture[index];
}
UClass* AEntity::GetBlueprint(Identifier value) {
	static UClass* uClass[static_cast<uint8>(Identifier::Length)] = { nullptr, };
	uint8 index = static_cast<uint8>(value);
	if (uClass[index] == nullptr) {
		FString name = StaticEnum<Identifier>()->GetNameStringByIndex(index);
		FString path = "/Game/Blueprints/BP_" + name + ".BP_" + name + "_C";
		uClass[index] = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *path));
	}
	if (!uClass[index]) UE_LOG(LogTemp, Warning, TEXT("Blueprint 'BP_%s' not found"), *ToFString(value));
	return uClass[index];
}
UMaterialInstance* AEntity::GetMaterialInstance() {
	static UMaterialInstance* uInstance = nullptr;
	if (uInstance == nullptr) {
		FString path = "/Game/Materials/MaterialInstance.MaterialInstance";
		uInstance = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), nullptr, *path));
	}
	if (!uInstance) UE_LOG(LogTemp, Warning, TEXT("Material 'MaterialInstance' not found"));
	return uInstance;
}



void AEntity::CreateMaterial(UStaticMeshComponent* component) {
	if (component == nullptr) component = spriteComponent;
	UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(GetMaterialInstance(), this);
	material->SetTextureParameterValue(TEXT("Texture"), GetTexture(identifier));
	component->SetMaterial(0, material);
}
void AEntity::SetSpriteIndex(UStaticMeshComponent* component, int32 value) {
	if (component == nullptr) component = spriteComponent;
	component->SetScalarParameterValueOnMaterials(TEXT("Index"), value);
}
void AEntity::SetSpriteXFlip(UStaticMeshComponent* component, bool value) {
	if (component == nullptr) component = spriteComponent;
	component->SetScalarParameterValueOnMaterials(TEXT("XFlip"), value ? 1.0f : 0.0f);
}
void AEntity::SetSpriteColor(UStaticMeshComponent* component, FVector value) {
	if (component == nullptr) component = spriteComponent;
	component->SetVectorParameterValueOnMaterials(TEXT("Color"), value);
}
void AEntity::SetSpriteLight(UStaticMeshComponent* component, float value) {
	if (component == nullptr) component = spriteComponent;
	component->SetScalarParameterValueOnMaterials(TEXT("Intensity"), value);
}



AEntity* AEntity::SpawnEntity(Identifier value, FVector location) {
	return GetWorld()->SpawnActor<AEntity>(GetBlueprint(value), location, FRotator::ZeroRotator);
}



AEntity::AEntity() {
 	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AParticle> ParticleClass(TEXT("/Game/Blueprints/BP_Particle_Dust.BP_Particle_Dust_C"));
	if (ParticleClass.Succeeded()) particleClass = ParticleClass.Class;
	static ConstructorHelpers::FClassFinder<AIndicator> IndicatorClass(TEXT("/Game/Blueprints/BP_Indicator.BP_Indicator_C"));
	if (IndicatorClass.Succeeded()) indicatorClass = IndicatorClass.Class;

	shadowComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShadowComponent"));
	shadowComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	shadowComponent->bCastHiddenShadow = true;
	shadowComponent->bHiddenInGame = true;
	shadowComponent->SetupAttachment(RootComponent);

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	meshComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 41.409618f));
	meshComponent->SetRelativeScale3D(FVector(1.28f, 1.28f, 1.28f));
	meshComponent->SetupAttachment(RootComponent);
}
void AEntity::BeginPlay() {
	Super::BeginPlay();
	
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	GetCharacterMovement()->GravityScale = HasTag(Tag::Floating) ? 0.0f : 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = speed;
	GetCharacterMovement()->MaxAcceleration = speed * 100;

	hitboxRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	hitboxHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * 2.0f;
}

void AEntity::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling()) {
		isFalling = true;
		fallSpeed = GetCharacterMovement()->Velocity.Z;
		if (fallSpeed < MaxFallSpeed) GetCharacterMovement()->Velocity.Z = MaxFallSpeed;
	}
	else if (isFalling) {
		if (fallSpeed < Particlethreshold) {
			Spawn<AParticle>(GetFootLocation() + FVector(0.0f, -hitboxRadius *  0.75f, 0.0f));
			Spawn<AParticle>(GetFootLocation() + FVector(0.0f, -hitboxRadius * -0.75f, 0.0f));
		}
		isFalling = false;
		fallSpeed = 0.0f;
	}

	for (uint8 i = 0; i < ToInt(Effect::Length); i++) {
	}
	UpdateEffect();

	spriteDelay += DeltaTime;
	UpdateSprite();
}
void AEntity::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}



template <typename T> T* AEntity::Spawn(FVector Location) {
	if constexpr (std::is_base_of<AIndicator, T>::value) {
		return GetWorld()->SpawnActor<T>(indicatorClass, Location, FRotator::ZeroRotator);
	}
	if constexpr (std::is_base_of<AParticle, T>::value) {
		return GetWorld()->SpawnActor<T>(particleClass, Location, FRotator::ZeroRotator);
	}
}



bool AEntity::HasTag(Tag value = ToEnum<Tag>(~0)) {
	return (ToInt(tag) & ToInt(value)) != 0;
}
void AEntity::AddTag(Tag value) {
	tag = ToEnum<Tag>(ToInt(tag) | ToInt(value));
}
void AEntity::RemoveTag(Tag value) {
	tag = ToEnum<Tag>(ToInt(tag) & ~ToInt(value));
}

bool AEntity::HasEffect(Effect value = ToEnum<Effect>(~0)) {
	return (ToInt(effect) & ToInt(value)) != 0;
}
bool AEntity::AddEffect(Effect value, float strength = 1.0f, float duration = 1.0f) {
	if ((ToInt(effectImmunity) & ToInt(value)) != 0) return false;
	effect = ToEnum<Effect>(ToInt(effect) | ToInt(value));
	return true;
}
void AEntity::RemoveEffect(Effect value) {
	effect = ToEnum<Effect>(ToInt(effect) & ~ToInt(value));
}

void AEntity::UpdateEffect() {
	for (uint8 i = 0; i < ToInt(Effect::Max); i++) {
	}
}

void AEntity::UpdateSprite() {
}



FVector AEntity::GetFootLocation() {
	return GetActorLocation() + FVector(-hitboxHeight * 0.5f, 0.0f, -hitboxHeight * 0.5f + 64.0f);
}