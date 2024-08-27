// all rights reversed GGWA


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet() {
	//델리게이트 부착하기 (GE 적용될때 호출되는 콜백 함수에 내용물 부착)
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) {
	//Widget controller 가 widget에게 값 전달을 하려면, Abilitycomponent로 부터 값을 받아와야 한다. 이를 위해 델리게이트 사용 (게임플레이 태그 사용)
	//게임플레이 태그 저장 최적화를 위해 Tarray 대신 FGameTagContainer를 사용
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	//TODO: Broadcast the tag to the widget controller
	EffectAssetTags.Broadcast(TagContainer);
}
