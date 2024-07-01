// all rights reversed GGWA

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	// 추상 개체는 instantiate 불가하므로 implement 하는 개체에서 override 사용해줄것.
	virtual void HighlightActor() override;//pure virtual as abstract
	virtual void UnHighlightActor() override;

protected:
	virtual void BeginPlay() override;
};
