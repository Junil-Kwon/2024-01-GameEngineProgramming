#include "Hero.h"
#include "Particle.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



AHero::AHero() {
	sensorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SensorComponent"));
	sensorComponent->InitSphereRadius(200.0f);
	sensorComponent->SetCollisionProfileName(TEXT("Sensor"));
	sensorComponent->SetupAttachment(RootComponent);

	springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	springArmComponent->SetRelativeLocation(FVector(-4000.0f, 0.0f, 4400.0f));
	springArmComponent->SetupAttachment(RootComponent);

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComponent->SetRelativeRotation(FRotator(-45.590382f, 0.0f, 0.0f));
	cameraComponent->PostProcessSettings.BloomIntensity = 0.0f;
	cameraComponent->FieldOfView = 15.0f;
	cameraComponent->SetupAttachment(springArmComponent);
}

void AHero::BeginPlay() {
	Super::BeginPlay();

	sensorComponent->OnComponentBeginOverlap.AddDynamic(this, &AHero::OnBeginSensed);
	sensorComponent->OnComponentEndOverlap  .AddDynamic(this, &AHero::OnEndSensed);
}

void AHero::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	UpdateInput(DeltaTime);

	//UE_LOG(LogTemp, Log, TEXT("%d"), sensed.Num());
}

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
			if (condition) Spawn<AParticle>(GetFootLocation());
			result = RotatorToVector(GetArrowComponent()->GetRelativeRotation());
			if (0.0f == spriteDelay) GetCharacterMovement()->MaxWalkSpeed = speed * 3.0f;
			if (0.2f <= spriteDelay) sprite = Sprite::Dodge1;
			break;
		case Sprite::Dodge1:
			condition = (int32)((spriteDelay + 0.9f) * 5) != (int32)((spriteDelay + 0.9f - DeltaTime) * 5);
			condition &= !GetCharacterMovement()->IsFalling();
			if (condition) Spawn<AParticle>(GetFootLocation());
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



void AHero::OnBeginSensed(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* PtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ACreature* creature = Cast<ACreature>(OtherActor);
	if (creature == nullptr) return;
	if (!sensed.Contains(creature) && creature->HasTag(Tag::Interactability)) {
		UE_LOG(LogTemp, Log, TEXT("begin"));
		sensed.Add(creature);
	}
}
void AHero::OnEndSensed(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	ACreature* creature = Cast<ACreature>(OtherActor);
	if (creature == nullptr) return;
	if (sensed.Contains(creature)) {
		UE_LOG(LogTemp, Log, TEXT("end"));
		sensed.Remove(creature);
	}
}



void AHero::UpdateSprite() {
	Super::UpdateSprite();

	if (sprite != spritePrev) {
		spritePrev = sprite;
		spriteDelay = 0.0f;
		switch (sprite) {
			case Sprite::Idle:   spriteIndex =  0; break;
			case Sprite::Move:   spriteIndex =  4; break;
			case Sprite::Jump0:  spriteIndex = 10; break;
			case Sprite::Jump1:  spriteIndex = 11; break;
			case Sprite::Jump2:  spriteIndex = 13; break;
			case Sprite::Dodge0: spriteIndex = 14; break;
			case Sprite::Dodge1: spriteIndex = 16; break;
			case Sprite::Defeat: spriteIndex = 20; break;
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
		if (spriteIndex == i) return;
		spriteIndex = i;
	}
	meshComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), spriteIndex);

	float y = 0.0f;
	switch (sprite) {
		case Sprite::Move:   y = direction.Y; break;
		case Sprite::Dodge0: y = RotatorToVector(GetArrowComponent()->GetRelativeRotation()).Y; break;
	}
	if (y != 0 && spriteXflip != (y < 0.0f)) {
		spriteXflip = y < 0.0f;
		meshComponent->SetScalarParameterValueOnMaterials(TEXT("XFlip"), spriteXflip ? 1.0f : 0.0f);
	}
}



void AHero::Die() {

}
