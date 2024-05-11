#include "Hero.h"
#include "Particle.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



AHero::AHero() {
	springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	springArmComponent->SetWorldLocation(FVector(-4000.0f, 0.0f, 4400.0f));
	springArmComponent->SetupAttachment(RootComponent);

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComponent->SetWorldRotation(FRotator(-45.590382f, 0.0f, 0.0f));
	cameraComponent->PostProcessSettings.BloomIntensity = 0.0f;
	cameraComponent->FieldOfView = 15.0f;
	cameraComponent->SetupAttachment(springArmComponent);
}



void AHero::BeginPlay() {
	Super::BeginPlay();
}



void AHero::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	UpdateInput(DeltaTime);
}



void AHero::OnBeginSensed(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* PtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnBeginSensed(OverlappedComponent, OtherActor, PtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ACreature* creature = Cast<ACreature>(OtherActor);
	if (creature == nullptr) return;
	if (!sensed.Contains(creature) && creature->HasTag(Tag::Interactability)) {
		sensed.Add(creature);
		if (selected == nullptr) Select(creature);
	}
}
void AHero::OnEndSensed(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnEndSensed(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	ACreature* creature = Cast<ACreature>(OtherActor);
	if (creature == nullptr) return;
	if (sensed.Contains(creature)) {
		sensed.Remove(creature);
		if (selected == creature) Select(nullptr);
	}
}

void AHero::Select(ACreature* creature) {
	if (selected != nullptr) {
		//selected->indicator->
		selected = nullptr;
	}
	if (creature != nullptr) {
		selected = creature;
		//selected->indicator->
	}
}
void AHero::UpdateSensor(float DeltaTime) {
	Super::UpdateSensor(DeltaTime);
	if (sensed.Num() == 0) return;

	float distance = GetSensorRange();
	for (int32 i = sensed.Num() - 1; -1 < i; i--) {
		if (sensed[i] == nullptr) {
			sensed.RemoveAt(i);
			continue;
		}
		float s = FVector::Dist(GetActorLocation(), sensed[i]->GetActorLocation());
		if (s < distance) {
			if (selected != sensed[i]) Select(sensed[i]);
			distance = s;
		}
	}
	if (!selected->HasTag(Tag::Interactability)) {
		sensed.Remove(selected);
		Select(nullptr);
	}
}



void AHero::UpdateSprite(float DeltaTime) {
	Super::UpdateSprite(DeltaTime);

	if (sprite != spritePrev) {
		spritePrev = sprite;
		spriteDelay = 0.0f;
		switch (sprite) {
		case Sprite::Idle:   SetSpriteIndex( 0); break;
		case Sprite::Move:   SetSpriteIndex( 4); break;
		case Sprite::Jump0:  SetSpriteIndex(10); break;
		case Sprite::Jump1:  SetSpriteIndex(11); break;
		case Sprite::Jump2:  SetSpriteIndex(13); break;
		case Sprite::Dodge0: SetSpriteIndex(14); break;
		case Sprite::Dodge1: SetSpriteIndex(16); break;
		case Sprite::Defeat: SetSpriteIndex(20); break;
		}
	}
	else {
		int32 i = 0;
		switch (sprite) {
			case Sprite::Idle:   i =  0 + static_cast<int32>(spriteDelay *  2) % 4; break;
			case Sprite::Move:   i =  4 + static_cast<int32>(spriteDelay * 10) % 6; break;
			case Sprite::Jump0:  i = 10 + static_cast<int32>(spriteDelay * 10); if (10 < i) i = 10; break;
			case Sprite::Jump1:  i = 11 + static_cast<int32>(spriteDelay * 10); if (13 < i) i = 13; break;
			case Sprite::Jump2:  i = 13 + static_cast<int32>(spriteDelay * 10); if (13 < i) i = 13; break;
			case Sprite::Dodge0: i = 14 + static_cast<int32>(spriteDelay * 10); if (15 < i) i = 15; break;
			case Sprite::Dodge1: i = 16 + static_cast<int32>(spriteDelay * 10); if (19 < i) i = 19; break;
			case Sprite::Defeat: i = 20 + static_cast<int32>(spriteDelay * 10); if (23 < i) i = 23; break;
		}
		SetSpriteIndex(i);
	}

	float y = 0.0f;
	switch (sprite) {
		case Sprite::Move:   y = direction.Y; break;
		case Sprite::Dodge0: y = RotatorToVector(GetArrowComponent()->GetRelativeRotation()).Y; break;
	}
	if (y != 0 && GetSpriteXFlip() != (y < 0.0f)) SetSpriteXFlip(!GetSpriteXFlip());
}



void AHero::Die() {
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
void AHero::Jump (bool pressed) { input[(uint8)Input::Jump ] = pressed; }
void AHero::Dodge(bool pressed) { input[(uint8)Input::Dodge] = pressed; }
void AHero::Act  (bool pressed) { input[(uint8)Input::Act  ] = pressed; }
void AHero::Swap (bool pressed) { input[(uint8)Input::Swap ] = pressed; }
void AHero::Menu (bool pressed) { input[(uint8)Input::Menu ] = pressed; }

void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Up",    IE_Pressed,  this, &AHero::Up,    true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Up",    IE_Released, this, &AHero::Up,    false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Down",  IE_Pressed,  this, &AHero::Down,  true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Down",  IE_Released, this, &AHero::Down,  false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Left",  IE_Pressed,  this, &AHero::Left,  true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Left",  IE_Released, this, &AHero::Left,  false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Right", IE_Pressed,  this, &AHero::Right, true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Right", IE_Released, this, &AHero::Right, false);

	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Jump",  IE_Pressed,  this, &AHero::Jump,  true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Jump",  IE_Released, this, &AHero::Jump,  false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Dodge", IE_Pressed,  this, &AHero::Dodge, true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Dodge", IE_Released, this, &AHero::Dodge, false);

	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Act",   IE_Pressed,  this, &AHero::Act,  true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Act",   IE_Released, this, &AHero::Act,  false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Swap",  IE_Pressed,  this, &AHero::Swap, true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Swap",  IE_Released, this, &AHero::Swap, false);
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Menu",  IE_Pressed,  this, &AHero::Menu, true );
	PlayerInputComponent->BindAction<TBaseDelegate<void, bool>>("Menu",  IE_Released, this, &AHero::Menu, false);
}
void AHero::UpdateInput(float DeltaTime) {
	FVector result = direction;
	bool condition = false;
	
	result.Normalize();
	switch (sprite) {
		case Sprite::Idle:
			if (!direction.IsZero()) sprite = Sprite::Move;
			if (input[(uint8)Input::Jump ] && !GetCharacterMovement()->IsFalling()) sprite = Sprite::Jump0;
			if (input[(uint8)Input::Dodge] && !GetCharacterMovement()->IsFalling()) sprite = Sprite::Dodge0;
			break;

		case Sprite::Move:
			if (!direction.IsZero()) GetArrowComponent()->SetWorldRotation(VectorToRotator(result));
			if (direction.IsZero()) sprite = Sprite::Idle;
			if (input[(uint8)Input::Jump ] && !GetCharacterMovement()->IsFalling()) sprite = Sprite::Jump0;
			if (input[(uint8)Input::Dodge] && !GetCharacterMovement()->IsFalling()) sprite = Sprite::Dodge0;
			break;

		case Sprite::Jump0:
			if (0.1f <= spriteDelay) sprite = Sprite::Jump1;
			break;
		case Sprite::Jump1:
			if (0.0f == spriteDelay) {
				if (!GetCharacterMovement()->IsFalling()) Super::Jump();
				else sprite = direction.IsZero() ? Sprite::Idle : Sprite::Move;
			}
			if (0.1f <= spriteDelay) sprite = Sprite::Jump2;
			break;
		case Sprite::Jump2:
			if (0.3f <= spriteDelay && !GetCharacterMovement()->IsFalling()) {
				sprite = direction.IsZero() ? Sprite::Idle : Sprite::Move;
			}
			break;

		case Sprite::Dodge0:
			condition = (int32)(spriteDelay * 20) != (int32)((spriteDelay - DeltaTime) * 20);
			condition &= !GetCharacterMovement()->IsFalling();
			if (condition) Spawn(Identifier::Dust, GetFootLocation());
			result = RotatorToVector(GetArrowComponent()->GetRelativeRotation());
			if (0.0f == spriteDelay) GetCharacterMovement()->MaxWalkSpeed = speed * 3.0f;
			if (0.2f <= spriteDelay) sprite = Sprite::Dodge1;
			break;
		case Sprite::Dodge1:
			condition = (int32)((spriteDelay + 0.9f) * 5) != (int32)((spriteDelay + 0.9f - DeltaTime) * 5);
			condition &= !GetCharacterMovement()->IsFalling();
			if (condition) Spawn(Identifier::Dust, GetFootLocation());
			result = RotatorToVector(GetArrowComponent()->GetRelativeRotation());
			if (0.0f == spriteDelay) GetCharacterMovement()->MaxWalkSpeed = speed;
			if (0.3f <= spriteDelay && input[(uint8)Input::Jump] && !GetCharacterMovement()->IsFalling()) sprite = Sprite::Jump0;
			if (0.4f <= spriteDelay) sprite = direction.IsZero() ? Sprite::Idle : Sprite::Move;
			break;

		case Sprite::Defeat:
			break;
	}
	AddMovementInput(result);
}
