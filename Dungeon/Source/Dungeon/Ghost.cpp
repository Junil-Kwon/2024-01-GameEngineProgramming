#include "Ghost.h"
#include "Creature.h"
#include "Interactor.h"
#include "Indicator.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "IngameUI.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"





// =============================================================================================================
// -
// =============================================================================================================

UTexture2D* AGhost::GetTexture(FString name) {
	FString path = "/Game/UI/" + name + "." + name;
	return LoadObject<UTexture2D>(nullptr, *path);
}
UClass* AGhost::GetWidget(FString name) {
	FString path = "/Game/Blueprints/BP_" + name + ".BP_" + name + "_C";
	return LoadClass<UUserWidget>(nullptr, *path);
}





// =============================================================================================================
// Initialization
// =============================================================================================================

AGhost::AGhost() {
	defaultSensorRange = 120.0f;

 	PrimaryActorTick.bCanEverTick = true;

	hitboxComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hitbox"));
	hitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(hitboxComponent);

	springComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
	springComponent->SetRelativeLocation(FVector(-4000.0f, 0.0f, 4400.0f));
	springComponent->SetupAttachment(RootComponent);

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->SetRelativeRotation(FRotator(-45.590382f, 0.0f, 0.0f));
	cameraComponent->PostProcessSettings.BloomIntensity = 0.0f;
	cameraComponent->FieldOfView = 15.0f;
	cameraComponent->SetupAttachment(springComponent);

	sensorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sensor"));
	sensorComponent->InitCapsuleSize(0.5f, 0.5f);
	sensorComponent->SetCollisionProfileName(TEXT("Sensor"));
	sensorComponent->SetupAttachment(RootComponent);
}
void AGhost::BeginPlay() {
	Super::BeginPlay();

	sensorComponent->OnComponentBeginOverlap.AddDynamic(this, &AGhost::OnSensorBeginOverlap);
	sensorComponent->OnComponentEndOverlap  .AddDynamic(this, &AGhost::OnSensorEndOverlap  );

	money = 0;

	ingameUI = static_cast<UIngameUI*>(CreateWidget<UUserWidget>(GetWorld(), GetWidget("IngameUI")));
	ingameUI->AddToViewport();

	focusing      = false;
	focusLocation = FVector::ZeroVector;
	focusEntity   = nullptr;
	shakeVertical = true;
	shakeStrength = 0.0f;
	shakeDuration = 0.0f;

	moneyTemp = 0;
	moneyIcon = 0;
	moneySize = 0;
	moneyXPos = ingameUI->moneyBackground->RenderTransform.Translation.X;
	moneyOpacity  = 1.0f;
	moneyDuration = 0.0f;

	keyboardUp    = false;
	keyboardDown  = false;
	keyboardLeft  = false;
	keyboardRight = false;
	keyboardSpace = false;
	keyboardShift = false;
	keyboardZ     = false;
	keyboardX     = false;
	keyboardC     = false;
	keyboardOpacity  = 1.0f;
	keyboardDuration = KeyboardShowDuration;

	FocusCameraOn(GetPlayer());
}

// =============================================================================================================
// Update
// =============================================================================================================

void AGhost::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UpdateCamera  (DeltaTime);
	UpdateSensor  (DeltaTime);
	UpdateMoney   (DeltaTime);
	UpdateKeyboard(DeltaTime);
}





// =============================================================================================================
// Camera
// =============================================================================================================

void AGhost::UpdateCamera(float DeltaTime) {
	FVector location = FVector::ZeroVector;
	if (!focusLocation.IsZero() || focusEntity != nullptr) {
		FVector target = focusEntity ? focusEntity->GetActorLocation() : focusLocation;
		if (focusing) location = target;
		else {
			float distance = FVector::Distance(GetActorLocation(), target);
			FVector locationDelta = (target - GetActorLocation()) / distance * CameraSpeed;
			location = GetActorLocation() + locationDelta * DeltaTime;
			if (FVector::Distance(location, target) < CameraSpeed * DeltaTime) focusing = true;
		}
	}
	shakeDuration = FMath::Max(shakeDuration - DeltaTime, 0.0f);
	if (shakeDuration) {
		FVector vector = FVector::ZeroVector;
		if (shakeVertical) vector.Y = FMath::RandRange(-1.0f, 1.0f) * shakeStrength;
		else 			   vector.Z = FMath::RandRange(-1.0f, 1.0f) * shakeStrength;
		location += FRotator(-48.590382f, 0.0f, 0.0f).RotateVector(vector);
	}
	SetActorLocation(location);
}
void AGhost::FocusCameraOn(AEntity* entity) {
	focusing      = false;
	focusEntity   = entity;
	if (entity   != nullptr) focusLocation = FVector::ZeroVector;
	SetSensorRange(static_cast<ACreature*>(entity));
}
void AGhost::FocusCameraOn(FVector location) {
	focusing      = false;
	focusEntity   = nullptr;
	focusLocation = location;
	SetSensorRange(static_cast<ACreature*>(nullptr));
}
void AGhost::UnfocusCamera() {
	focusing      = false;
	focusEntity   = nullptr;
	focusLocation = FVector::ZeroVector;
	SetSensorRange(static_cast<ACreature*>(nullptr));
}
void AGhost::ShakeCamera(float strength, float duration, bool vertical) {
	shakeStrength = FMath::Max(strength, 0.0f);
	shakeDuration = FMath::Max(duration, 0.0f);
	shakeVertical = vertical;
}

// =============================================================================================================
// Sensor
// =============================================================================================================

bool AGhost::UpdateSensor(float DeltaTime) {
	if (sensorArray.Num() == 0) return false;

	for (int32 i = sensorArray.Num() - 1; -1 < i; i--) {
		if (sensorArray[i] == nullptr) {
			sensorArray.RemoveAt(i);
			continue;
		}
	}
	AEntity* entity = nullptr;
	float nearest = sensorRange;
	if (focusing && focusEntity) for (int32 i = 0; i < sensorArray.Num(); i++) {
		float distance = FVector::Distance(sensorArray[i]->GetActorLocation(), GetActorLocation());
		if (sensorArray[i]->HasTag(Tag::Interactability) && distance < nearest) {
			entity = sensorArray[i];
			nearest = distance;
		}
	}
	SetSelected(entity);
	return true;
}

float AGhost::GetSensorRange() {
	return sensorRange;
}
void  AGhost::SetSensorRange(AEntity* entity) {
	if (!(entity->IsA(ACreature::StaticClass()))) {
		sensorRange = defaultSensorRange;
		sensorComponent->SetCapsuleSize(defaultSensorRange, 0.0f);
	}
	else {
		ACreature* creature = static_cast<ACreature*>(entity);
		sensorRange = creature->GetMagnetRange();
		float hitboxRadius = creature->GetHitboxRadius();
		float hitboxHeight = creature->GetHitboxHeight();
		sensorComponent->SetCapsuleRadius    (sensorRange);
		sensorComponent->SetCapsuleHalfHeight(sensorRange + hitboxHeight * 0.5f - hitboxRadius);
	}
}
void AGhost::OnSensorBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {
	AEntity* entity = static_cast<AEntity*>(OtherActor);
	if (OtherActor->IsA(AEntity::StaticClass()) && !sensorArray.Contains(entity)) sensorArray.Add(entity);
}
void AGhost::OnSensorEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	AEntity* entity = static_cast<AEntity*>(OtherActor);
	if (OtherActor->IsA(AEntity::StaticClass()) && sensorArray.Contains(entity)) {
		sensorArray.Remove(entity);
		if (GetSelected() == entity) SetSelected(nullptr);
	}
}

// =============================================================================================================
// Select
// =============================================================================================================

bool     AGhost::HasSelected() { return selected != nullptr; };
AEntity* AGhost::GetSelected() { return selected; }
void AGhost::SetSelected(AEntity* entity) {
	if (selected == entity) return;
	if (selected != nullptr) {
		if (interactor) interactor->Despawn();
	}
	if (entity != nullptr) {
		if (!interactor) {
			interactor = static_cast<AInteractor*>(entity->Spawn(Identifier::Interactor));
			interactor->OnInteract(entity);
		}
	}
	selected = entity;
}

// =============================================================================================================
// Money
// =============================================================================================================

int32 AGhost::GetMoney() {
	return money;
}
void  AGhost::AdjustMoney(int32 value) {
	money += value;
}
void AGhost::UpdateMoney(float DeltaTime) {
	moneyDuration = FMath::Max(moneyDuration - DeltaTime, 0.0f);

	if (moneyTemp != money) moneyDuration = 5.0f;
	if (moneyOpacity != FMath::Min(moneyDuration, 1.0f)) {
		moneyOpacity  = FMath::Min(moneyDuration, 1.0f);
		ingameUI->moneyText      ->SetOpacity((moneyDuration - 0.00f) * 2);
		ingameUI->moneyIcon      ->SetOpacity((moneyDuration - 0.00f) * 2);
		ingameUI->moneyBackground->SetOpacity((moneyDuration - 0.00f) * 2);
	}
	if (moneyTemp != money) {
		moneyTemp += (moneyTemp < money ? 1 : -1) * DeltaTime * 20.0f;
		moneyTemp = FMath::Clamp(moneyTemp, 0.0f, float(money));
		ingameUI->moneyText->SetText(FText::FromString(FString::FromInt(moneyTemp)));

		int32 moneyIconTemp = 0;
		if      (moneyTemp <   5) moneyIconTemp = 1;
		else if (moneyTemp <  20) moneyIconTemp = 2;
		else if (moneyTemp <  50) moneyIconTemp = 3;
		else if (moneyTemp < 200) moneyIconTemp = 4;
		else if (moneyTemp < 500) moneyIconTemp = 5;
		else                      moneyIconTemp = 6;
		if (moneyIcon != moneyIconTemp) {
			moneyIcon = moneyIconTemp;
			ingameUI->moneyIcon->SetBrushFromTexture(GetTexture("MoneyIcon" + FString::FromInt(moneyIconTemp)));
		}
		int32 moneySizeTemp = 0;
		for (int32 i = 1; true; i *= 10) {
			if (int32(moneyTemp / i) == 0) break;
			moneySizeTemp++;
		}
		if (moneySize != moneySizeTemp) {
			moneySize = moneySizeTemp;
			ingameUI->moneyBackground->SetRenderTranslation(FVector2D(moneyXPos - 36 * (moneySize - 1), 0.0f));
		}
	}
}

// =============================================================================================================
// Keyboard
// =============================================================================================================

void AGhost::UpdateKeyboard(float DeltaTime) {
	keyboardDuration = FMath::Max(keyboardDuration - DeltaTime, 0.0f);

	if (keyboardOpacity != FMath::Min(keyboardDuration, 1.0f)) {
		keyboardOpacity  = FMath::Min(keyboardDuration, 1.0f);
		ingameUI->keyboardUp        ->SetOpacity((keyboardDuration - 0.50f) * 2);
		ingameUI->keyboardDown      ->SetOpacity((keyboardDuration - 0.50f) * 2);
		ingameUI->keyboardLeft      ->SetOpacity((keyboardDuration - 0.50f) * 2);
		ingameUI->keyboardRight     ->SetOpacity((keyboardDuration - 0.50f) * 2);
		ingameUI->keyboardSpace     ->SetOpacity((keyboardDuration - 0.50f) * 2);
		ingameUI->keyboardShift     ->SetOpacity((keyboardDuration - 0.50f) * 2);
		ingameUI->keyboardZ         ->SetOpacity((keyboardDuration - 0.50f) * 2);
		ingameUI->keyboardX         ->SetOpacity((keyboardDuration - 0.50f) * 2);
		ingameUI->keyboardC         ->SetOpacity((keyboardDuration - 0.50f) * 2);
		ingameUI->keyboardBackground->SetOpacity((keyboardDuration - 0.00f) * 2);
	}
	if (keyboardUp    != inputDirection[0]) {
		keyboardUp    = inputDirection[0];
		FString name = "KeyboardUp"    + FString(keyboardUp    ? "2" : "1");
		ingameUI->keyboardUp->SetBrushFromTexture(GetTexture(name));
	}
	if (keyboardDown  != inputDirection[1]) {
		keyboardDown  = inputDirection[1];
		FString name = "KeyboardDown"  + FString(keyboardDown  ? "2" : "1");
		ingameUI->keyboardDown->SetBrushFromTexture(GetTexture(name));
	}
	if (keyboardLeft  != inputDirection[2]) {
		keyboardLeft  = inputDirection[2];
		FString name = "KeyboardLeft"  + FString(keyboardLeft  ? "2" : "1");
		ingameUI->keyboardLeft->SetBrushFromTexture(GetTexture(name));
	}
	if (keyboardRight != inputDirection[3]) {
		keyboardRight = inputDirection[3];
		FString name = "KeyboardRight" + FString(keyboardRight ? "2" : "1");
		ingameUI->keyboardRight->SetBrushFromTexture(GetTexture(name));
	}
	if (keyboardSpace != inputAction[(uint8)Action::Jump]) {
		keyboardSpace = inputAction[(uint8)Action::Jump];
		FString name = "KeyboardSpace" + FString(keyboardSpace ? "2" : "1");
		ingameUI->keyboardSpace->SetBrushFromTexture(GetTexture(name));
	}
	if (keyboardShift != inputAction[(uint8)Action::Dash]) {
		keyboardShift = inputAction[(uint8)Action::Dash];
		FString name = "KeyboardShift" + FString(keyboardShift ? "2" : "1");
		ingameUI->keyboardShift->SetBrushFromTexture(GetTexture(name));
	}
	if (keyboardZ     != inputAction[(uint8)Action::Attack]) {
		keyboardZ     = inputAction[(uint8)Action::Attack];
		FString name = "KeyboardZ"     + FString(keyboardZ     ? "2" : "1");
		ingameUI->keyboardZ->SetBrushFromTexture(GetTexture(name));
	}
	if (keyboardX     != inputAction[(uint8)Action::Defend]) {
		keyboardX     = inputAction[(uint8)Action::Defend];
		FString name = "KeyboardX"     + FString(keyboardX     ? "2" : "1");
		ingameUI->keyboardX->SetBrushFromTexture(GetTexture(name));
	}
	if (keyboardC     != false) {
		keyboardC     = false;
		FString name = "KeyboardC"     + FString(keyboardC     ? "2" : "1");
		ingameUI->keyboardC->SetBrushFromTexture(GetTexture(name));
	}
}





// =============================================================================================================
// Entity
// =============================================================================================================

TArray<AEntity*>* AGhost::GetObjectPool(Identifier value) {
	return &objectPool[static_cast<uint8>(value)];
}

AEntity* AGhost::GetPlayer() {
	return player;
}
void AGhost::SetPlayer(AEntity* value) {
	if (player == value) return;
	if (player != nullptr) {
		if (player->HasTag(Tag::Player)) player->RemoveTag(Tag::Player);
		FocusCameraOn(nullptr);
	}
	if (value  != nullptr) {
		if (!value->HasTag(Tag::Player)) value->AddTag(Tag::Player);
		FocusCameraOn(value);
	}
	if (player == nullptr && value != nullptr) OnPlayerSpawned  ();
	if (player != nullptr && value == nullptr) OnPlayerDestroyed();
	player = value;
}

void AGhost::OnPlayerSpawned() {
	UE_LOG(LogTemp, Warning, TEXT("Player Spawned.  "));
}
void AGhost::OnPlayerDestroyed() {
	UE_LOG(LogTemp, Warning, TEXT("Player Destroyed."));
}





// =============================================================================================================
// Input
// =============================================================================================================

void AGhost::PUp    () { inputDirection[0] = true;  direction.X =  1.0f;                            }
void AGhost::RUp    () { inputDirection[0] = false; direction.X = inputDirection[1] ? -1.0f : 0.0f; }
void AGhost::PDown  () { inputDirection[1] = true;  direction.X = -1.0f;                            }
void AGhost::RDown  () { inputDirection[1] = false; direction.X = inputDirection[0] ?  1.0f : 0.0f; }
void AGhost::PLeft  () { inputDirection[2] = true;  direction.Y = -1.0f;                            }
void AGhost::RLeft  () { inputDirection[2] = false; direction.Y = inputDirection[3] ?  1.0f : 0.0f; }
void AGhost::PRight () { inputDirection[3] = true;  direction.Y =  1.0f;                            }
void AGhost::RRight () { inputDirection[3] = false; direction.Y = inputDirection[2] ? -1.0f : 0.0f; }
void AGhost::PJump  () { inputAction[(uint8)Action::Jump  ] = true;  }
void AGhost::RJump  () { inputAction[(uint8)Action::Jump  ] = false; }
void AGhost::PDash  () { inputAction[(uint8)Action::Dash  ] = true;  }
void AGhost::RDash  () { inputAction[(uint8)Action::Dash  ] = false; }
void AGhost::PAttack() { inputAction[(uint8)Action::Attack] = true;  }
void AGhost::RAttack() { inputAction[(uint8)Action::Attack] = false; }
void AGhost::PDefend() { inputAction[(uint8)Action::Defend] = true;  }
void AGhost::RDefend() { inputAction[(uint8)Action::Defend] = false; }
void AGhost::PMenu  () { }
void AGhost::RMenu  () { }

void AGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Up",     IE_Pressed,  this, &AGhost::PUp    );
	PlayerInputComponent->BindAction("Up",     IE_Released, this, &AGhost::RUp    );
	PlayerInputComponent->BindAction("Down",   IE_Pressed,  this, &AGhost::PDown  );
	PlayerInputComponent->BindAction("Down",   IE_Released, this, &AGhost::RDown  );
	PlayerInputComponent->BindAction("Left",   IE_Pressed,  this, &AGhost::PLeft  );
	PlayerInputComponent->BindAction("Left",   IE_Released, this, &AGhost::RLeft  );
	PlayerInputComponent->BindAction("Right",  IE_Pressed,  this, &AGhost::PRight );
	PlayerInputComponent->BindAction("Right",  IE_Released, this, &AGhost::RRight );

	PlayerInputComponent->BindAction("Jump",   IE_Pressed,  this, &AGhost::PJump  );
	PlayerInputComponent->BindAction("Jump",   IE_Released, this, &AGhost::RJump  );
	PlayerInputComponent->BindAction("Dash",   IE_Pressed,  this, &AGhost::PDash  );
	PlayerInputComponent->BindAction("Dash",   IE_Released, this, &AGhost::RDash  );

	PlayerInputComponent->BindAction("Attack", IE_Pressed,  this, &AGhost::PAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AGhost::RAttack);
	PlayerInputComponent->BindAction("Defend", IE_Pressed,  this, &AGhost::PDefend);
	PlayerInputComponent->BindAction("Defend", IE_Released, this, &AGhost::RDefend);
	PlayerInputComponent->BindAction("Menu",   IE_Pressed,  this, &AGhost::PMenu  );
	PlayerInputComponent->BindAction("Menu",   IE_Released, this, &AGhost::RMenu  );
}
bool AGhost::GetInput(Action value) { return inputAction[static_cast<uint8>(value)]; }
FVector AGhost::GetInputDirection() { return direction; }
