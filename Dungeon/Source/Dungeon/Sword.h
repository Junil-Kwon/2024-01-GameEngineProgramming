#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Sword.generated.h"





UCLASS()
class DUNGEON_API ASword : public AWeapon {
	GENERATED_BODY()
	




	// Action
private:
	int32 pattern;
public:
	virtual bool UpdateAction(float DeltaTime) override;
};
