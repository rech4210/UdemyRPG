// all rights reversed GGWA

//#include "InputMappingContext.h"
#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

// Tick 함수 오버라이딩 (근데 왜 Tick 함수를 override 하지? 유니티의 Update같은거 아닌가)
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	
}

// 하이라이트에 사용될 커서 함수 정의
void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	// if Cast hit Not IEnemyInterface, Then return null;
	ThisActor = CursorHit.GetActor(); // no needs cast cause it wrapped

	/*
	 * Line trace from cursor. there are several scenarios;
	 * A. LastActor is null && This Actor is null;
	 *		- Do nothing
	 * B. LastActor is null && ThisActor is valid;
	 *		- Highlight ThisActor
	 * C. LastActor is valid && ThisActor is null
	 *		- UnHighlight LastActor
	 * D. Both actors are valid, but LastActor != ThisActor;
	 *		- UnHIghlist LastActor, and Highligh ThisActor
	 * E. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */

	if(LastActor == nullptr){
		if(ThisActor != nullptr){
			// case B
			ThisActor->HighlightActor();
		}
		else {
			//both are null, do nothing - case A
		}
	}
	else {// LastActor is valid
		if(ThisActor == nullptr) {
			//case C
			LastActor->UnHighlightActor();
		}
		else { // both actors are valid
			if(LastActor != ThisActor) {
				//case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else {
				//Case E - do nothing
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext); //해당 변수가 세팅되지 않을 경우 크래시
	//mapping context를 사용할 수 있는 subsystem 싱글톤
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	/*InputComponent -> */ //input 데이터로 액터의 행동 제어
	// 컨트롤러의 setup 설정. EnhancedInputComponent에 BindAction으로 상태에 따라 함수 호출
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

//컨트롤러에 매핑될 Move 함수
void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
	//X,Y 2D 벡터,액터의 회전값
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	// 이런 미친 엔진은 X,Y,Z를 냅두고 Roll, Pitch, Yaw로 부르는걸 모자라,  인자 순서도 Y,Z,X다 미친 엔진
	// Yaw 회전을 기준으로 하는 새로운 회전값
	const FRotator YawRotation(0.f, Rotation.Yaw,0.f);
	
	// Yaw 회전 기준으로 전방,우측 벡터를 계산
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>()){
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);// Pitch
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X); // Roll
	}
}


