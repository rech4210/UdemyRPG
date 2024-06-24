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

	//�𸮾� ��ü���� wrapper�ϴ� �����ͷ� ��ü�� ��� ����, �����ϴ���, lazy coding ���� ����� ��.
	UPROPERTY(EditAnywhere, Category = "combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
