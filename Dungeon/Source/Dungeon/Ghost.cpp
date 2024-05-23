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
