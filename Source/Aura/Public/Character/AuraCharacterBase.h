// all rights reversed GGWA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//언리얼 개체들을 wrapper하는 포인터로 개체에 어떻게 참조, 해제하는지, lazy coding 등의 기능을 함.
	UPROPERTY(EditAnywhere, Category = "combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
