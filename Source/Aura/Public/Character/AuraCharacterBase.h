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

	//�𸮾� ��ü���� wrapper�ϴ� �����ͷ� ��ü�� ��� ����, �����ϴ���, lazy coding ���� ����� ��.
	UPROPERTY(EditAnywhere, Category = "combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	//store for attribute, state
	UPROPERTY() // �Ƹ� �����ϴ� �����ΰ�?
// �ƴ� �̰� UAuraAbilitySystemComponent: UAbilitySystemComponent�ε� �θ� Ŭ���� ide �ȶ߳�

	// �� �κ��� ���� initialize ���� ����.
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
};
