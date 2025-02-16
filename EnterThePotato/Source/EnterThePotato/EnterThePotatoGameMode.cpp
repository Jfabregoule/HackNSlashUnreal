// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnterThePotatoGameMode.h"
#include "EnterThePotatoPlayerController.h"
#include "EnterThePotatoCharacter.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EPC_CharacterBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AEnterThePotatoGameMode::AEnterThePotatoGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AEnterThePotatoPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void AEnterThePotatoGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("GameplayGameMode is now active!"));

	if (BackgroundMusic)
	{
		UGameplayStatics::PlaySound2D(this, BackgroundMusic, 0.1f);
	}

	if (HealthBarClass)
	{
		HealthBar = CreateWidget<UUserWidget>(GetWorld(), HealthBarClass);

		if (HealthBarClass)
		{
			HealthBar = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarClass);
			if (HealthBar)
			{
				HealthBar->AddToViewport();
				UE_LOG(LogTemp, Warning, TEXT("HealthBar successfully created and added to viewport."));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create HealthBar widget!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("HealthBarClass is NULL! Cannot create HealthBar widget."));
		}
	}
}

UUserWidget* AEnterThePotatoGameMode::getHealthBar()
{
	return HealthBar;
}
