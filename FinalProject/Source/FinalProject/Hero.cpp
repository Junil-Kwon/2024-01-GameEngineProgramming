#include "Hero.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"



AHero::AHero() {
 	PrimaryActorTick.bCanEverTick = true;

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	sphereComponent->InitSphereRadius(24.0f);
	//sphereComponent->SetCollisionProfileName(TEXT("Player"));
	SetRootComponent(sphereComponent);

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	meshComponent->SetWorldScale3D(FVector(1.28f, 1.28f, 1.28f));
	meshComponent->SetupAttachment(RootComponent);


}



void AHero::BeginPlay() {
	Super::BeginPlay();
	
}



void AHero::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UpdateSprite(DeltaTime);

	direction.Normalize();
	if (direction.IsZero()) {
		state = State::Idle;
	}
	else {
		state = State::Move;
		if (direction.Y != 0 && xflip != (direction.Y < 0.0f)) {
			xflip = direction.Y < 0.0f;
			meshComponent->SetScalarParameterValueOnMaterials(TEXT("XFlip"), xflip ? 1.0f : 0.0f);
			UE_LOG(LogTemp, Log, TEXT("%d"), xflip ? 1 : 0);
		}
		FVector location = GetActorLocation() + direction * speed * DeltaTime;
		SetActorLocation(location, true);
	}
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
			case State::Jump:   i = 10 + static_cast<int32>(delay * 10); if (i < 13) i = 13; break;
			case State::Dodge:  i = 14 + static_cast<int32>(delay * 10); if (i < 19) i = 19; break;
			case State::Defeat: i = 20 + static_cast<int32>(delay * 10); if (i < 23) i = 23; break;
		}
		if (index == i) return;
		index = i;
	}
	meshComponent->SetScalarParameterValueOnMaterials(TEXT("Index"), index);
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