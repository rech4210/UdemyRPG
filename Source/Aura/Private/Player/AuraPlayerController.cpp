// all rights reversed GGWA

//#include "InputMappingContext.h"
#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
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

