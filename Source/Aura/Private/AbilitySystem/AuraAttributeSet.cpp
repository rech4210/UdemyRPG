// all rights reversed GGWA

#include "AbilitySystem/AuraAttributeSet.h"
#include "GameFramework//Character.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet() {
	InitHealth((50.f));
	InitMaxHealth(100.f);
	InitMana(10.f);
	InitMaxMana(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//register variable for replication
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always/*When regist in server, update and set it replicated*/);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

// Attribute 가 변경될때 사전에 호출되는 함수 override하여 사용. 실제 값 변경을 다루지 않음
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {
	Super::PreAttributeChange(Attribute, NewValue);
	
	if(Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue,0.f, GetMaxHealth());
		// UE_LOG(LogTemp, Warning,TEXT("Health: %f"), NewValue);
	}

	if(Attribute == GetManaAttribute()) {
		NewValue = FMath::Clamp(NewValue,0.f, GetMaxMana());
		// UE_LOG(LogTemp, Warning,TEXT("Mana: %f"), NewValue);
	}
}


void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const {
	// source = causer of the effect, Target = target of the effect (owner of this AS)
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	// wrapper in smart pointer
	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid()) {
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		
		if(Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr) {
			if(const APawn * Pawn = Cast<APawn>(Props.SourceAvatarActor)) {
				Props.SourceController = Pawn->GetController();
			}
		}
		if(Props.SourceController) {
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	// smart pointer로 Wrapping되어 있으므로 Get 사용
	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) {
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		//다운 캐스팅
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		//AActor로 부터 AbilitySystem 가져오기
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}


// 값이 변경된 이후 실행되는 콜백 함수 Data에는 Effect spec, Effect context, EvaluatedData, Target등의 정보가 저장되어 있다.
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	//이제 post 함수가 실행될 때 캐릭터에 대한 자세한 정보에 쉽게 접근할 수 있게 됨.
}

// if(Data.EvaluatedData.Attribute == GetHealthAttribute()) {
// 	UE_LOG(LogTemp,Warning, TEXT("Health from GetHealth() : %f"), GetHealth());
// 	UE_LOG(LogTemp,Warning, TEXT("Manitude : %f"), Data.EvaluatedData.Magnitude);
// }

//call when replicated health variable
void UAuraAttributeSet::Onrep_Health(const FGameplayAttributeData& OldHealth) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth); // helper macro that can be used in RepNotify functions to handle attributes that will be predictively modified by clients.
}

void UAuraAttributeSet::Onrep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::Onrep_Mana(const FGameplayAttributeData& OldMana) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::Onrep_MaxMana(const FGameplayAttributeData& OldMaxMana) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}



// template <typename T>
// void UAuraAttributeSet::Onrep_Attribute(const FGameplayAttributeData data, const T& Attribute) const {
// 	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, , Attribute);
// }
