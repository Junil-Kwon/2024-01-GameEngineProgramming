#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Money.generated.h"





UCLASS()
class DUNGEON_API AMoney : public AEntity {
	GENERATED_BODY()





	// Initialization
public:
	AMoney();
protected:
	virtual void BeginPlay() override;
};
