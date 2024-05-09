#include "Hero.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



AHero::AHero() {
	springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	springArmComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 600.0f));
	springArmComponent->SetupAttachment(RootComponent);

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComponent->SetRelativeRotation(FRotator(-45.590382f, 0.0f, 0.0f));
	cameraComponent->SetupAttachment(springArmComponent);
}

void AHero::BeginPlay() {
	Super::BeginPlay();
}

void AHero::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	direction.Normalize();
	switch (sprite) {
		case Sprite::Idle:
			if (!direction.IsZero()) sprite = Sprite::Move;
			if (input[(uint8)Input::Jump]  && !GetCharacterMovement()->IsFalling()) sprite = Sprite::Jump;
			if (input[(uint8)Input::Dodge] && !GetCharacterMovement()->IsFalling()) sprite = Sprite::Dodge;
			break;

		case Sprite::Move:
			if (!direction.IsZero()) GetArrowComponent()->SetWorldRotation(VectorToRotator(direction));
			AddMovementInput(direction);
			if (direction.IsZero()) sprite = Sprite::Idle;
			if (input[(uint8)Input::Jump]  && !GetCharacterMovement()->IsFalling()) sprite = Sprite::Jump;
			if (input[(uint8)Input::Dodge] && !GetCharacterMovement()->IsFalling()) sprite = Sprite::Dodge;
			break;

		case Sprite::Jump:
			AddMovementInput(direction);
			if (0.1f < delay && delay < 0.2f && !GetCharacterMovement()->IsFalling()) Super::Jump();
			if (0.2f < delay && !GetCharacterMovement()->IsFalling()) {
				sprite = direction.IsZero() ? Sprite::Idle : Sprite::Move;
			}
			break;

		case Sprite::Dodge:
			if (delay == 0.0f) GetCharacterMovement()->MaxWalkSpeed = speed * 2.0f;
			if (0.3f < delay) GetCharacterMovement()->MaxWalkSpeed = speed;
			AddMovementInput(RotatorToVector(GetArrowComponent()->GetRelativeRotation()));
			if (0.4f < delay && input[(uint8)Input::Jump] && !GetCharacterMovement()->IsFalling()) sprite = Sprite::Jump;
			if (0.6f < delay) sprite = direction.IsZero() ? Sprite::Idle : Sprite::Move;
			break;

		case Sprite::Defeat:
			break;
	}
	UpdateSprite(DeltaTime);
}

void AHero::UpdateSprite(float DeltaTime) {
	delay += DeltaTime;
	if (sprite != spritePrev) {
		spritePrev = sprite;
		delay = 0.0f;
		switch (sprite) {
			case Sprite::Idle:   index =  0; break;
			case Sprite::Move:   index =  4; break;
			case Sprite::Jump:   index = 10; break;
			case Sprite::Dodge:  index = 14; break;
			case Sprite::Defeat: index = 20; break;
		}
	}
	else {
		int32 i = 0;
		switch (sprite) {
			case Sprite::Idle:   i =  0 + static_cast<int32>(delay *  2) % 4; break;
			case Sprite::Move:   i =  4 + static_cast<int32>(delay * 10) % 6; break;
			case Sprite::Jump:   i = 10 + static_cast<int32>(delay * 10); if (13 < i) i = 13; break;
			case Sprite::Dodge:  i = 14 + static_cast<int32>(delay * 10); if (19 < i) i = 19; break;
			case Sprite::Defeat: i = 20 + static_cast<int32>(delay * 10); if (23 < i) i = 23; break;
		}
		if (index == i) return;
		index = i;
	}
	meshComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), index);
	if (sprite == Sprite::Move && direction.Y != 0 && xflip != (direction.Y < 0.0f)) {
		xflip = direction.Y < 0.0f;
		meshComponent->SetScalarParameterValueOnMaterials(TEXT("XFlip"), xflip ? 1.0f : 0.0f);
	}
}

void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Up",     IE_Pressed,  this, &AHero::Up,     true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Up",     IE_Released, this, &AHero::Up,     false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Down",   IE_Pressed,  this, &AHero::Down,   true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Down",   IE_Released, this, &AHero::Down,   false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Left",   IE_Pressed,  this, &AHero::Left,   true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Left",   IE_Released, this, &AHero::Left,   false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Right",  IE_Pressed,  this, &AHero::Right,  true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Right",  IE_Released, this, &AHero::Right,  false);

	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Jump",   IE_Pressed,  this, &AHero::Jump,   true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Jump",   IE_Released, this, &AHero::Jump,   false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Dodge",  IE_Pressed,  this, &AHero::Dodge,  true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Dodge",  IE_Released, this, &AHero::Dodge,  false);

	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Action", IE_Pressed,  this, &AHero::Action, true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Action", IE_Released, this, &AHero::Action, false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Swap",   IE_Pressed,  this, &AHero::Swap,   true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Swap",   IE_Released, this, &AHero::Swap,   false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Menu",   IE_Pressed,  this, &AHero::Menu,   true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Menu",   IE_Released, this, &AHero::Menu,   false);
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
