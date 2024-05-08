#include "Hero.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"



AHero::AHero() {
 	PrimaryActorTick.bCanEverTick = true;

	//capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComponent"));
	//capsuleComponent->SetCollisionProfileName(TEXT("Entity"));
	//SetRootComponent(capsuleComponent);

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	meshComponent->SetWorldRotation(FRotator(0.0f, 90.0f, 41.409618f));
	meshComponent->SetWorldScale3D(FVector(1.28f, 1.28f, 1.28f));
	meshComponent->SetupAttachment(RootComponent);

	GetCharacterMovement()->MaxWalkSpeed = speed;
	GetCharacterMovement()->MaxAcceleration = speed * 100;
	GetCharacterMovement()->JumpZVelocity = 800;
	GetCharacterMovement()->GravityScale = 3.0f;
}



void AHero::BeginPlay() {
	Super::BeginPlay();
}



FRotator VectorToRotator(FVector vector) {
	return FRotator(0.0f, 90.0f - FMath::RadiansToDegrees(FMath::Atan2(vector.X, vector.Y)), 0.0f);
}
FVector RotatorToVector(FRotator rotator) {
	float rad = FMath::DegreesToRadians(rotator.Yaw);
	return FVector(FMath::Cos(rad), FMath::Sin(rad), 0.0f);
}

void AHero::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	direction.Normalize();
	switch (state) {
		case State::Idle:
			if (!direction.IsZero()) state = State::Move;
			if (input[(uint8)Input::Jump]  && !GetCharacterMovement()->IsFalling()) state = State::Jump;
			if (input[(uint8)Input::Dodge] && !GetCharacterMovement()->IsFalling()) state = State::Dodge;
			break;

		case State::Move:
			if (!direction.IsZero()) GetArrowComponent()->SetWorldRotation(VectorToRotator(direction));
			AddMovementInput(direction);
			if (direction.IsZero()) state = State::Idle;
			if (input[(uint8)Input::Jump]  && !GetCharacterMovement()->IsFalling()) state = State::Jump;
			if (input[(uint8)Input::Dodge] && !GetCharacterMovement()->IsFalling()) state = State::Dodge;
			break;

		case State::Jump:
			AddMovementInput(direction);
			if (0.1f < delay && delay < 0.2f && !GetCharacterMovement()->IsFalling()) Super::Jump();
			if (0.2f < delay && !GetCharacterMovement()->IsFalling()) {
				state = direction.IsZero() ? State::Idle : State::Move;
			}
			break;

		case State::Dodge:
			if (delay == 0.0f) GetCharacterMovement()->MaxWalkSpeed = speed * 2.0f;
			if (0.3f < delay) GetCharacterMovement()->MaxWalkSpeed = speed;
			AddMovementInput(RotatorToVector(GetArrowComponent()->GetRelativeRotation()));
			if (0.4f < delay && input[(uint8)Input::Jump] && !GetCharacterMovement()->IsFalling()) state = State::Jump;
			if (0.6f < delay) state = direction.IsZero() ? State::Idle : State::Move;
			break;

		case State::Defeat:
			break;
	}
	UpdateSprite(DeltaTime);
}

void AHero::UpdateSprite(float DeltaTime) {
	delay += DeltaTime;
	if (state != statePrev) {
		statePrev = state;
		delay = 0.0f;
		switch (state) {
			case State::Idle:   index =  0; break;
			case State::Move:   index =  4; break;
			case State::Jump:   index = 10; break;
			case State::Dodge:  index = 14; break;
			case State::Defeat: index = 20; break;
		}
	}
	else {
		int32 i = 0;
		switch (state) {
			case State::Idle:   i =  0 + static_cast<int32>(delay *  2) % 4; break;
			case State::Move:   i =  4 + static_cast<int32>(delay * 10) % 6; break;
			case State::Jump:   i = 10 + static_cast<int32>(delay * 10); if (13 < i) i = 13; break;
			case State::Dodge:  i = 14 + static_cast<int32>(delay * 10); if (19 < i) i = 19; break;
			case State::Defeat: i = 20 + static_cast<int32>(delay * 10); if (23 < i) i = 23; break;
		}
		if (index == i) return;
		index = i;
	}
	meshComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), index);
	if (state == State::Move && direction.Y != 0 && xflip != (direction.Y < 0.0f)) {
		xflip = direction.Y < 0.0f;
		meshComponent->SetScalarParameterValueOnMaterials(TEXT("XFlip"), xflip ? 1.0f : 0.0f);
	}
}



void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInput) {
	Super::SetupPlayerInputComponent(PlayerInput);

	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Up",     IE_Pressed,  this, &AHero::Up,     true );
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Up",     IE_Released, this, &AHero::Up,     false);
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Down",   IE_Pressed,  this, &AHero::Down,   true );
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Down",   IE_Released, this, &AHero::Down,   false);
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Left",   IE_Pressed,  this, &AHero::Left,   true );
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Left",   IE_Released, this, &AHero::Left,   false);
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Right",  IE_Pressed,  this, &AHero::Right,  true );
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Right",  IE_Released, this, &AHero::Right,  false);

	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Jump",   IE_Pressed,  this, &AHero::Jump,   true );
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Jump",   IE_Released, this, &AHero::Jump,   false);
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Dodge",  IE_Pressed,  this, &AHero::Dodge,  true );
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Dodge",  IE_Released, this, &AHero::Dodge,  false);

	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Action", IE_Pressed,  this, &AHero::Action, true );
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Action", IE_Released, this, &AHero::Action, false);
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Swap",   IE_Pressed,  this, &AHero::Swap,   true );
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Swap",   IE_Released, this, &AHero::Swap,   false);
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Menu",   IE_Pressed,  this, &AHero::Menu,   true );
	PlayerInput->BindAction<TBaseDelegate<void, bool>>("Menu",   IE_Released, this, &AHero::Menu,   false);
}

void AHero::Up    (bool pressed) {
	input[(uint8)Input::Up    ] = pressed;
	direction.X = pressed ?  1.0f : input[(uint8)Input::Down ] ? -1.0f : 0.0f;
}
void AHero::Down  (bool pressed) {
	input[(uint8)Input::Down  ] = pressed;
	direction.X = pressed ? -1.0f : input[(uint8)Input::Up   ] ?  1.0f : 0.0f;
}
void AHero::Left  (bool pressed) {
	input[(uint8)Input::Left  ] = pressed;
	direction.Y = pressed ? -1.0f : input[(uint8)Input::Right] ?  1.0f : 0.0f;
}
void AHero::Right (bool pressed) {
	input[(uint8)Input::Right ] = pressed;
	direction.Y = pressed ?  1.0f : input[(uint8)Input::Left ] ? -1.0f : 0.0f;
}

void AHero::Jump  (bool pressed) { input[(uint8)Input::Jump  ] = pressed; }
void AHero::Dodge (bool pressed) { input[(uint8)Input::Dodge ] = pressed; }

void AHero::Action(bool pressed) { input[(uint8)Input::Action] = pressed; }
void AHero::Swap  (bool pressed) { input[(uint8)Input::Swap  ] = pressed; }
void AHero::Menu  (bool pressed) { input[(uint8)Input::Menu  ] = pressed; }