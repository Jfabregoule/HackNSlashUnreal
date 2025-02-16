// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "EPGameInstance.h"
#include "EnterThePotatoPlayerController.h"


bool UMainMenuWidget::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    SetupGameModeReference();

    if (PlayButton)
        PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);

    if (OptionsButton)
        OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsClicked);

    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);

    return true;
}

void UMainMenuWidget::OnPlayClicked()
{
    UEPGameInstance* gameInstance = Cast<UEPGameInstance>(GetGameInstance());
    UE_LOG(LogTemp, Warning, TEXT("Bouton Jouer cliqué !"));
    UWorld* World = GetWorld();
    if (World && gameInstance)
    {
        gameInstance->LoadGameLevel(World);
    }
}

void UMainMenuWidget::OnOptionsClicked()
{
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    if (!PlayerController->KeyMappingWidgetClass) return;
    GameModeRef->getMainMenuInstance()->RemoveFromParent();
    UUserWidget* KeyMappingUI = CreateWidget<UUserWidget>(this, PlayerController->KeyMappingWidgetClass);
    if (KeyMappingUI)
    {
        UE_LOG(LogTemp, Warning, TEXT("Bouto cliqué !"));
        KeyMappingUI->AddToViewport();
        PlayerController->SetInputMode(FInputModeUIOnly());
        PlayerController->bShowMouseCursor = true;
    }
}

void UMainMenuWidget::OnQuitClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Bouton Quitter cliqué !"));
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMainMenuWidget::SetupGameModeReference()
{
    AGameModeBase* GM = GetWorld()->GetAuthGameMode();
    if (GM)
    {
        GameModeRef = Cast<AMainMenuGameMod>(GM);
    }
}
