// all rights reversed GGWA


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

// �浹�� �÷��̾� �Ǵ� ���ʹ̿� ������ �� �ֵ��� �ϱ�. IAbilitySystemInterface�� �����ϱ�
void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {

	//TODO : Change this to apply a Gameplay Effect. For now, using const_cast as a hack!
	if(IAbilitySystemInterface * ASCInterface = Cast<IAbilitySystemInterface>(OtherActor)) {
		const UAuraAttributeSet* AuraAttributeSet =  Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()/*return reference of instance*/));
		/*GetAttributeSet이 const 반환이므로 const 붙여줄것.*/
		// use when use before remove 
		UAuraAttributeSet* MutableAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);
		MutableAuraAttributeSet->SetHealth((AuraAttributeSet->GetHealth() + 25.f));
		Destroy();
	}
}


void AAuraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	
	
}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	// ��ü�� ������ �� ��� ����Ǵ� �ݹ� �Լ� ��� (��������Ʈ)
	//DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_SixParams( FComponentBeginOverlapSignature, UPrimitiveComponent, OnComponentBeginOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult &, SweepResult);
	// ���ε� �ϱ� ���ؼ� OnOverlap �Լ��� �Ű������� ���� ���ǵ� �Ķ���� �������� �����Ѵ�.
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this,&AAuraEffectActor::EndOverlap);
}