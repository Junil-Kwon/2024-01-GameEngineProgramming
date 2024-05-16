#include "Ghost.h"

#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"



AGhost::AGhost() {
 	PrimaryActorTick.bCanEverTick = true;

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	sphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(sphereComponent);

	springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArmComponent->SetWorldLocation(FVector(-4000.0f, 0.0f, 4400.0f));
	springArmComponent->SetupAttachment(RootComponent);

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->SetWorldRotation(FRotator(-45.590382f, 0.0f, 0.0f));
	cameraComponent->PostProcessSettings.BloomIntensity = 0.0f;
	cameraComponent->FieldOfView = 15.0f;
	cameraComponent->SetupAttachment(springArmComponent);
}



void AGhost::BeginPlay() {
	Super::BeginPlay();
}



void AGhost::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}



float AGhost::GetWorldSpeed() { return worldSpeed; }
void  AGhost::SetWorldSpeed(float value) { worldSpeed = value; }



ACreature* AGhost::GetPlayer() {
	return player;
}
void AGhost::SetPlayer(ACreature* value) {
	if (player != nullptr) player->RemoveTag(Tag::Player);
	player = value;
}



void AGhost::InputMoveUp   (bool pressed) {
	inputDirection[Up   ] = pressed;
	direction.X = pressed ?  1.0f : (inputDirection[Down ] ? -1.0f : 0.0f);
}
void AGhost::InputMoveDown (bool pressed) {
	inputDirection[Down ] = pressed;
	direction.X = pressed ? -1.0f : (inputDirection[Up   ] ?  1.0f : 0.0f);
}
void AGhost::InputMoveLeft (bool pressed) {
	inputDirection[Left ] = pressed;
	direction.Y = pressed ? -1.0f : (inputDirection[Right] ?  1.0f : 0.0f);
}
void AGhost::InputMoveRight(bool pressed) {
	inputDirection[Right] = pressed;
	direction.Y = pressed ?  1.0f : (inputDirection[Left ] ? -1.0f : 0.0f);
}
void AGhost::InputJump  (bool pressed) { inputAction[(uint8)Action::Jump  ] = pressed; }
void AGhost::InputDash  (bool pressed) { inputAction[(uint8)Action::Dash  ] = pressed; }
void AGhost::InputAttack(bool pressed) { inputAction[(uint8)Action::Attack] = pressed; }
void AGhost::InputDefend(bool pressed) { inputAction[(uint8)Action::Defend] = pressed; }
void AGhost::InputMenu  (bool pressed) {  }

void AGhost::SetupPlayerInputComponent(UInputComponent* input) {
	Super::SetupPlayerInputComponent(input);

	input->BindAction<TBaseDelegate<void, bool>>("MoveUp",    IE_Pressed,  this, &AGhost::InputMoveUp,    true );
	input->BindAction<TBaseDelegate<void, bool>>("MoveUp",    IE_Released, this, &AGhost::InputMoveUp,    false);
	input->BindAction<TBaseDelegate<void, bool>>("MoveDown",  IE_Pressed,  this, &AGhost::InputMoveDown,  true );
	input->BindAction<TBaseDelegate<void, bool>>("MoveDown",  IE_Released, this, &AGhost::InputMoveDown,  false);
	input->BindAction<TBaseDelegate<void, bool>>("MoveLeft",  IE_Pressed,  this, &AGhost::InputMoveLeft,  true );
	input->BindAction<TBaseDelegate<void, bool>>("MoveLeft",  IE_Released, this, &AGhost::InputMoveLeft,  false);
	input->BindAction<TBaseDelegate<void, bool>>("MoveRight", IE_Pressed,  this, &AGhost::InputMoveRight, true );
	input->BindAction<TBaseDelegate<void, bool>>("MoveRight", IE_Released, this, &AGhost::InputMoveRight, false);

	input->BindAction<TBaseDelegate<void, bool>>("Jump",      IE_Pressed,  this, &AGhost::InputJump,      true );
	input->BindAction<TBaseDelegate<void, bool>>("Jump",      IE_Released, this, &AGhost::InputJump,      false);
	input->BindAction<TBaseDelegate<void, bool>>("Dash",      IE_Pressed,  this, &AGhost::InputDash,      true );
	input->BindAction<TBaseDelegate<void, bool>>("Dash",      IE_Released, this, &AGhost::InputDash,      false);

	input->BindAction<TBaseDelegate<void, bool>>("Action",    IE_Pressed,  this, &AGhost::InputAttack,    true );
	input->BindAction<TBaseDelegate<void, bool>>("Action",    IE_Released, this, &AGhost::InputAttack,    false);
	input->BindAction<TBaseDelegate<void, bool>>("Defend",    IE_Pressed,  this, &AGhost::InputDefend,    true );
	input->BindAction<TBaseDelegate<void, bool>>("Defend",    IE_Released, this, &AGhost::InputDefend,    false);
	input->BindAction<TBaseDelegate<void, bool>>("Menu",      IE_Pressed,  this, &AGhost::InputMenu,      true );
	input->BindAction<TBaseDelegate<void, bool>>("Menu",      IE_Released, this, &AGhost::InputMenu,      false);

}
bool AGhost::GetInput(Action value) {
	return inputAction[static_cast<uint8>(value)];
}
FVector AGhost::GetInputDirection() {
	return direction;
}
