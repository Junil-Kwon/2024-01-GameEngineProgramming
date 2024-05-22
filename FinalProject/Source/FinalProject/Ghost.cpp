#include "Ghost.h"

#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AGhost::AGhost() {
 	PrimaryActorTick.bCanEverTick = true;

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	sphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(sphereComponent);

	springComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
	springComponent->SetRelativeLocation(FVector(-4000.0f, 0.0f, 4400.0f));
	springComponent->SetupAttachment(RootComponent);

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->SetRelativeRotation(FRotator(-45.590382f, 0.0f, 0.0f));
	cameraComponent->PostProcessSettings.BloomIntensity = 0.0f;
	cameraComponent->FieldOfView = 15.0f;
	cameraComponent->SetupAttachment(springComponent);
}
void AGhost::BeginPlay() {
	Super::BeginPlay();
}

// =============================================================================================================
// Update
// =============================================================================================================

void AGhost::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (GetPlayer() != nullptr) SetActorLocation(player->GetActorLocation());
}





// =============================================================================================================
// Input
// =============================================================================================================

void AGhost::MoveUp(bool pressed) {
	inputDirection[0] = pressed;
	direction.X = pressed ? 1.0f : (inputDirection[1] ? -1.0f : 0.0f);
}
void AGhost::MoveDown(bool pressed) {
	inputDirection[1] = pressed;
	direction.X = pressed ? -1.0f : (inputDirection[0] ? 1.0f : 0.0f);
}
void AGhost::MoveLeft(bool pressed) {
	inputDirection[2] = pressed;
	direction.Y = pressed ? -1.0f : (inputDirection[3] ? 1.0f : 0.0f);
}
void AGhost::MoveRight(bool pressed) {
	inputDirection[3] = pressed;
	direction.Y = pressed ? 1.0f : (inputDirection[2] ? -1.0f : 0.0f);
}
void AGhost::Jump  (bool pressed) { inputAction[(uint8)Action::Jump  ] = pressed; }
void AGhost::Dash  (bool pressed) { inputAction[(uint8)Action::Dash  ] = pressed; }
void AGhost::Attack(bool pressed) { inputAction[(uint8)Action::Attack] = pressed; }
void AGhost::Defend(bool pressed) { inputAction[(uint8)Action::Defend] = pressed; }
void AGhost::Menu  (bool pressed) {  }

void AGhost::SetupPlayerInputComponent(UInputComponent* input) {
	Super::SetupPlayerInputComponent(input);

	input->BindAction<TBaseDelegate<void, bool>>("MoveUp",    IE_Pressed,  this, &AGhost::MoveUp,    true );
	input->BindAction<TBaseDelegate<void, bool>>("MoveUp",    IE_Released, this, &AGhost::MoveUp,    false);
	input->BindAction<TBaseDelegate<void, bool>>("MoveDown",  IE_Pressed,  this, &AGhost::MoveDown,  true );
	input->BindAction<TBaseDelegate<void, bool>>("MoveDown",  IE_Released, this, &AGhost::MoveDown,  false);
	input->BindAction<TBaseDelegate<void, bool>>("MoveLeft",  IE_Pressed,  this, &AGhost::MoveLeft,  true );
	input->BindAction<TBaseDelegate<void, bool>>("MoveLeft",  IE_Released, this, &AGhost::MoveLeft,  false);
	input->BindAction<TBaseDelegate<void, bool>>("MoveRight", IE_Pressed,  this, &AGhost::MoveRight, true );
	input->BindAction<TBaseDelegate<void, bool>>("MoveRight", IE_Released, this, &AGhost::MoveRight, false);

	input->BindAction<TBaseDelegate<void, bool>>("Jump",      IE_Pressed,  this, &AGhost::Jump,      true );
	input->BindAction<TBaseDelegate<void, bool>>("Jump",      IE_Released, this, &AGhost::Jump,      false);
	input->BindAction<TBaseDelegate<void, bool>>("Dash",      IE_Pressed,  this, &AGhost::Dash,      true );
	input->BindAction<TBaseDelegate<void, bool>>("Dash",      IE_Released, this, &AGhost::Dash,      false);

	input->BindAction<TBaseDelegate<void, bool>>("Attack",    IE_Pressed,  this, &AGhost::Attack,    true );
	input->BindAction<TBaseDelegate<void, bool>>("Attack",    IE_Released, this, &AGhost::Attack,    false);
	input->BindAction<TBaseDelegate<void, bool>>("Defend",    IE_Pressed,  this, &AGhost::Defend,    true );
	input->BindAction<TBaseDelegate<void, bool>>("Defend",    IE_Released, this, &AGhost::Defend,    false);
	input->BindAction<TBaseDelegate<void, bool>>("Menu",      IE_Pressed,  this, &AGhost::Menu,      true );
	input->BindAction<TBaseDelegate<void, bool>>("Menu",      IE_Released, this, &AGhost::Menu,      false);
}
bool AGhost::GetInput(Action value) {
	return inputAction[static_cast<uint8>(value)];
}
FVector AGhost::GetInputDirection() {
	return direction;
}





// =============================================================================================================
// Player
// =============================================================================================================

AEntity* AGhost::GetPlayer() {
	return player;
}
void AGhost::SetPlayer(AEntity* value) {
	if (player != nullptr) player->RemoveTag(Tag::Player);
	else if (value != nullptr) OnPlayerSpawned();
	player = value;
}

void AGhost::OnPlayerSpawned() {
	UE_LOG(LogTemp, Warning, TEXT("Player Spawned."));
}
void AGhost::OnPlayerDestroyed() {
	UE_LOG(LogTemp, Warning, TEXT("Player Destroyed."));
}
