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



void AGhost::Up   (bool pressed) {
	input[(uint8)Input::Up    ] = pressed;
	direction.X = pressed ?  1.0f : input[(uint8)Input::Down ] ? -1.0f : 0.0f;
}
void AGhost::Down (bool pressed) {
	input[(uint8)Input::Down  ] = pressed;
	direction.X = pressed ? -1.0f : input[(uint8)Input::Up   ] ?  1.0f : 0.0f;
}
void AGhost::Left (bool pressed) {
	input[(uint8)Input::Left  ] = pressed;
	direction.Y = pressed ? -1.0f : input[(uint8)Input::Right] ?  1.0f : 0.0f;
}
void AGhost::Right(bool pressed) {
	input[(uint8)Input::Right ] = pressed;
	direction.Y = pressed ?  1.0f : input[(uint8)Input::Left ] ? -1.0f : 0.0f;
}
void AGhost::Space(bool pressed) { input[(uint8)Input::Space] = pressed; }
void AGhost::Shift(bool pressed) { input[(uint8)Input::Shift] = pressed; }
void AGhost::Z    (bool pressed) { input[(uint8)Input::Z    ] = pressed; }
void AGhost::X    (bool pressed) { input[(uint8)Input::X    ] = pressed; }
void AGhost::C    (bool pressed) { input[(uint8)Input::C    ] = pressed; }

void AGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Up",    IE_Pressed,  this, &AGhost::Up   , true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Up",    IE_Released, this, &AGhost::Up   , false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Down",  IE_Pressed,  this, &AGhost::Down , true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Down",  IE_Released, this, &AGhost::Down , false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Left",  IE_Pressed,  this, &AGhost::Left , true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Left",  IE_Released, this, &AGhost::Left , false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Right", IE_Pressed,  this, &AGhost::Right, true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Right", IE_Released, this, &AGhost::Right, false);

	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Jump",  IE_Pressed,  this, &AGhost::Space, true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Jump",  IE_Released, this, &AGhost::Space, false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Dodge", IE_Pressed,  this, &AGhost::Shift, true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Dodge", IE_Released, this, &AGhost::Shift, false);

	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Act",   IE_Pressed,  this, &AGhost::Z    , true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Act",   IE_Released, this, &AGhost::Z    , false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Swap",  IE_Pressed,  this, &AGhost::X    , true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Swap",  IE_Released, this, &AGhost::X    , false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Menu",  IE_Pressed,  this, &AGhost::C    , true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Menu",  IE_Released, this, &AGhost::C    , false);

}
bool AGhost::GetInput(Action value) {
	return input[static_cast<uint8>(value)];
}
FVector AGhost::GetInputDirection() {
	return direction;
}
