// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnterThePotatoPlayerController.h"
#include "EnterThePotatoCharacter.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerKeysSave.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AEnterThePotatoPlayerController::AEnterThePotatoPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	speed = 800.0f;
	startSpeed = 0.0f;
	sprintSpeed = 1200.0f;
	isPaused = false;
}

void AEnterThePotatoPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	bShowMouseCursor = true;
	if (APawn* ControlledPawn = GetPawn())
	{
		ACharacter* CharacterPawn = Cast<ACharacter>(ControlledPawn);
		if (CharacterPawn && CharacterPawn->GetCharacterMovement())
		{
			CharacterPawn->GetCharacterMovement()->MaxWalkSpeed = speed;
		}
	}
	
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputMode);
}

void AEnterThePotatoPlayerController::RebindKey(UInputAction* Action, FKey NewKey)
{
	if (!Action) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Enhanced Input Subsystem!"));
		return;
	}

	
	Subsystem->RemoveMappingContext(DefaultMappingContext);

	
	for (const FEnhancedActionKeyMapping& Mapping : DefaultMappingContext->GetMappings())
	{
		if (Mapping.Action == Action) 
		{
			DefaultMappingContext->UnmapKey(Action, Mapping.Key); 
			DefaultMappingContext->MapKey(Action, NewKey); 
			break; 
		}
	}

	Subsystem->AddMappingContext(DefaultMappingContext, 0);

	PlayerKeyBindings.Add(Action, NewKey);
	SavePlayerKeyBindings();
}

void AEnterThePotatoPlayerController::SavePlayerKeyBindings()
{
	UPlayerKeysSave* SaveGameInstance = Cast<UPlayerKeysSave>(UGameplayStatics::CreateSaveGameObject(UPlayerKeysSave::StaticClass()));

	for (const auto& Binding : PlayerKeyBindings)
	{
		SaveGameInstance->SavedKeyBindings.Add(Binding.Key->GetFName().ToString(), Binding.Value);
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "PlayerKeyBindings", 0);
}

void AEnterThePotatoPlayerController::LoadPlayerKeyBindings()
{
	if (UGameplayStatics::DoesSaveGameExist("PlayerKeyBindings", 0))
	{
		UPlayerKeysSave* SaveGameInstance = Cast<UPlayerKeysSave>(UGameplayStatics::LoadGameFromSlot("PlayerKeyBindings", 0));

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

		if (!Subsystem)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get Enhanced Input Subsystem!"));
			return;
		}

		Subsystem->RemoveMappingContext(DefaultMappingContext);

		for (const auto& SavedBinding : SaveGameInstance->SavedKeyBindings)
		{
			// Find the matching action from the stored key bindings
			for (TPair<UInputAction*, FKey>& Binding : PlayerKeyBindings)
			{
				if (Binding.Key->GetFName().ToString() == SavedBinding.Key)
				{
					UInputAction* ActionKey = Binding.Key;

					// Remove old key mapping
					DefaultMappingContext->UnmapKey(ActionKey, Binding.Value);

					// Apply new key mapping
					DefaultMappingContext->MapKey(ActionKey, SavedBinding.Value);

					// Update local PlayerKeyBindings map
					Binding.Value = SavedBinding.Value;

					break;  // Exit loop after finding the matching action
				}
			}
		}

		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

UUserWidget* AEnterThePotatoPlayerController::getPauseMenuInstance()
{
	return PauseMenuInstance;
}

void AEnterThePotatoPlayerController::setPauseMenuInstanceNULL()
{
	PauseMenuInstance = nullptr;
}

AEnterThePotatoPlayerController* AEnterThePotatoPlayerController::getController(UObject* WorldContext)
{
	return Cast<AEnterThePotatoPlayerController>(UGameplayStatics::GetPlayerController(WorldContext, 0));
}

void AEnterThePotatoPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	UE_LOG(LogTemp, Warning, TEXT("SETUP INPUT COMPONENT"))

	// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enhanced Input Subsystem found"));

			if (DefaultMappingContext)
			{
				UE_LOG(LogTemp, Warning, TEXT("Adding Default Mapping Context"));
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("DefaultMappingContext is null!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get Enhanced Input Subsystem!"));
		}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup keyboard input events
		EnhancedInputComponent->BindAction(SetDestinationKeyboardAction, ETriggerEvent::Triggered, this, &AEnterThePotatoPlayerController::onMove);
		EnhancedInputComponent->BindAction(SetDestinationSprint, ETriggerEvent::Started, this, &AEnterThePotatoPlayerController::onSprint);
		EnhancedInputComponent->BindAction(SetDestinationSprint, ETriggerEvent::Completed, this, &AEnterThePotatoPlayerController::notOnSprint);
		EnhancedInputComponent->BindAction(SetDestinationZoom, ETriggerEvent::Triggered, this, &AEnterThePotatoPlayerController::zoomCamera);
		EnhancedInputComponent->BindAction(SetDestinationUnZoom, ETriggerEvent::Triggered, this, &AEnterThePotatoPlayerController::unZoomCamera);
		EnhancedInputComponent->BindAction(SetDestinationPause, ETriggerEvent::Completed, this, &AEnterThePotatoPlayerController::pauseGame);

		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Started, this, &AEnterThePotatoPlayerController::PickUpActionHandler);
		EnhancedInputComponent->BindAction(HitAction, ETriggerEvent::Started, this, &AEnterThePotatoPlayerController::HitActionHandler);
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Started, this, &AEnterThePotatoPlayerController::ThrowActionHandler);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	InputComponent->bBlockInput = false;

}

void AEnterThePotatoPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!isPaused)
		RotatePlayerToMouse();
}

void AEnterThePotatoPlayerController::onMove(const FInputActionValue & Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		FVector Forward = FVector(1, 0, 0);
		FVector Right = FVector(0, 1, 0);

		FVector MovementDirection = (Forward * MovementVector.Y + Right * MovementVector.X);

		ControlledPawn->AddMovementInput(MovementDirection);
	}
}

void AEnterThePotatoPlayerController::onSprint()
{
	UE_LOG(LogTemp, Warning, TEXT("SPRINT"))
	if (APawn* ControlledPawn = GetPawn())
	{
		ACharacter* CharacterPawn = Cast<ACharacter>(ControlledPawn);
		if (CharacterPawn && CharacterPawn->GetCharacterMovement())
		{
			startSpeed = CharacterPawn->GetCharacterMovement()->MaxWalkSpeed;
			CharacterPawn->GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
			/*if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
				-1, 15.0f, FColor::Red,
				FString::Printf(TEXT("Max Walk Speed: %f"), CharacterPawn->GetCharacterMovement()->MaxWalkSpeed)
				);
			}*/
		}
	}
}

void AEnterThePotatoPlayerController::zoomCamera()
{
	AEnterThePotatoCharacter* MyCharacter = Cast<AEnterThePotatoCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->zoomCameraCharacter();
	}
}

void AEnterThePotatoPlayerController::unZoomCamera()
{
	AEnterThePotatoCharacter* MyCharacter = Cast<AEnterThePotatoCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->unZoomCameraCharacter();
	}
}

void AEnterThePotatoPlayerController::pauseGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Pause button pressed!"));
	if (isPaused)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unpausing game..."));

		if (PauseMenuInstance)
		{
			PauseMenuInstance->RemoveFromParent();
			PauseMenuInstance = nullptr;
		}

		UGameplayStatics::SetGamePaused(GetWorld(), false);
		isPaused = false;

		// Re-enable normal input
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pausing game..."));

		if (!PauseMenuInstance && PauseMenuClass)
		{
			PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
			if (PauseMenuInstance)
			{
				PauseMenuInstance->AddToViewport();
			}
		}

		UGameplayStatics::SetGamePaused(GetWorld(), true);
		isPaused = true;

		// Allow input while paused
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}
}

void AEnterThePotatoPlayerController::notOnSprint()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		ACharacter* CharacterPawn = Cast<ACharacter>(ControlledPawn);
		if (CharacterPawn && CharacterPawn->GetCharacterMovement())
		{
			CharacterPawn->GetCharacterMovement()->MaxWalkSpeed = startSpeed;
		}
	}
}

void AEnterThePotatoPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AEnterThePotatoPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AEnterThePotatoPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AEnterThePotatoPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}
 
void AEnterThePotatoPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AEnterThePotatoPlayerController::RotatePlayerToMouse()
{
	FVector WorldLocation, WorldDirection;

	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		APawn* ControlledPawn = GetPawn();
		if (ControlledPawn)
		{
			FVector PawnLocation = ControlledPawn->GetActorLocation();

			float Distance = (PawnLocation.Z - WorldLocation.Z) / WorldDirection.Z;
			FVector TargetPosition = WorldLocation + WorldDirection * Distance;

			FVector DirectionToTarget = (TargetPosition - PawnLocation).GetSafeNormal2D();
			FRotator LookAtRotation = FRotationMatrix::MakeFromX(DirectionToTarget).Rotator();

			FRotator CurrentRotation = ControlledPawn->GetActorRotation();
			FRotator TargetRotation = FRotator(0, LookAtRotation.Yaw, 0);

			float InterpSpeed = 5.0f;
			FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);

			ControlledPawn->SetActorRotation(SmoothRotation);
		}
	}
}

#pragma region Weapon system

void AEnterThePotatoPlayerController::PickUpActionHandler(const FInputActionValue& Value)
{
	
	AEnterThePotatoCharacter* MyCharacter = Cast<AEnterThePotatoCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->PickUpAction();
	}
}

void AEnterThePotatoPlayerController::HitActionHandler(const FInputActionValue& Value)
{
	AEnterThePotatoCharacter* MyCharacter = Cast<AEnterThePotatoCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->HitAction();
	}
}

void AEnterThePotatoPlayerController::ThrowActionHandler(const FInputActionValue& Value)
{
	AEnterThePotatoCharacter* MyCharacter = Cast<AEnterThePotatoCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->ThrowAction();
	}
}

#pragma endregion




