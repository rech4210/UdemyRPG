// all rights reversed GGWA

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface 
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//언리얼 개체들을 wrapper하는 포인터로 개체에 어떻게 참조, 해제하는지, lazy coding 등의 기능을 함.
	UPROPERTY(EditAnywhere, Category = "combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	//store for attribute, state
	UPROPERTY() // 아마 래핑하는 과정인가?
// 아니 이거 UAuraAbilitySystemComponent: UAbilitySystemComponent인데 부모 클래스 ide 안뜨네

	// 이 부분은 아직 initialize 되지 않음.
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
};
