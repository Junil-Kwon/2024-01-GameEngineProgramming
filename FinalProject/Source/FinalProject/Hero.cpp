#include "Hero.h"



AHero::AHero() {
	// init stats
}

void AHero::BeginPlay() {
	Super::BeginPlay();
}



bool AHero::VerifyAction(Action value) {
	if (GetAction() == value || GetAction() == Action::Defeat) return false;
	if (value == Action::Idle || value == Action::Move || value == Action::Defeat) return true;
	switch (GetAction()) {
	case Action::Idle: return (GetAction() != value);
	case Action::Move: return (GetAction() != value);
	case Action::Jump:
		switch (value) {
		case Action::Jump:   return false;
		case Action::Dash:   return false;
		case Action::Attack: return true;
		case Action::Defend: return true;
		}
		break;
	case Action::Dash:
		switch (value) {
		case Action::Jump:   return false;
		case Action::Dash:   return false;
		case Action::Attack: return false;
		case Action::Defend: return false;
		}
		break;
	case Action::Attack:
		switch (value) {
		case Action::Jump:   return true;
		case Action::Dash:   return true;
		case Action::Attack: return true;
		case Action::Defend: return true;
		}
		break;
	case Action::Defend:
		switch (value) {
		case Action::Jump:   return true;
		case Action::Dash:   return true;
		case Action::Attack: return true;
		case Action::Defend: return true;
		}
		break;
	}
	return false;
}
bool AHero::UpdateInputs(float DeltaTime) {
	if (!Super::UpdateInputs(DeltaTime)) return false;
	
	return true;
}
bool AHero::UpdateAction(float DeltaTime) {
	if (!Super::UpdateAction(DeltaTime)) return false;

	switch (GetAction()) {
	case Action::Idle:
		SetSpriteIndex(nullptr,  0 + static_cast<int32>(actionDelay *  2) % 4);
		break;
	case Action::Move:
		SetSpriteIndex(nullptr,  4 + static_cast<int32>(actionDelay * 10) % 6);
		break;
	case Action::Jump:
		SetSpriteIndex(nullptr, FMath::Min(10 + static_cast<int32>(actionDelay * 10), 13));
		break;
	case Action::Dash:
		SetSpriteIndex(nullptr, FMath::Min(14 + static_cast<int32>(actionDelay * 10), 19));
		break;
	case Action::Attack:
		
		break;
	case Action::Defend:
		
		break;
	case Action::Defeat:
		SetSpriteIndex(nullptr, FMath::Min(20 + static_cast<int32>(actionDelay * 10), 23));
		break;
	}

	return true;
}



void AHero::OnSensorBeginOverlap(
	UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnSensorBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
void AHero::OnSensorEndOverlap(
	UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnSensorEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
}




void AHero::OnDie() {
}
