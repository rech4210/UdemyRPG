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

// Tick �Լ� �������̵� (�ٵ� �� Tick �Լ��� override ����? ����Ƽ�� Update������ �ƴѰ�)
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	
}

// ���̶���Ʈ�� ���� Ŀ�� �Լ� ����
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

	check(AuraContext); //�ش� ������ ���õ��� ���� ��� ũ����
	//mapping context�� ����� �� �ִ� subsystem �̱���
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
	/*InputComponent -> */ //input �����ͷ� ������ �ൿ ����
	// ��Ʈ�ѷ��� setup ����. EnhancedInputComponent�� BindAction���� ���¿� ���� �Լ� ȣ��
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

//��Ʈ�ѷ��� ���ε� Move �Լ�
void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
	//X,Y 2D ����,������ ȸ����
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	// �̷� ��ģ ������ X,Y,Z�� ���ΰ� Roll, Pitch, Yaw�� �θ��°� ���ڶ�,  ���� ������ Y,Z,X�� ��ģ ����
	// Yaw ȸ���� �������� �ϴ� ���ο� ȸ����
	const FRotator YawRotation(0.f, Rotation.Yaw,0.f);
	
	// Yaw ȸ�� �������� ����,���� ���͸� ���
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>()){
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);// Pitch
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X); // Roll
	}
}


